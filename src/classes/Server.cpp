#include "Server.hpp"
#include <signal.h>
#include "../pr.cpp"

bool progOver = false;

Server::Server(const char *ip, int port, int address_family, int type, std::string password) {
		std::cout << "Server constructor." << std::endl;
		FD_ZERO(&_readfds);
		_hintlen = 0;
		_executor = new Executor(this);
		_port = port;
		_address_family = address_family;
		_type = type;
		memset(_host, 0, NI_MAXHOST);
		memset(_service, 0, NI_MAXSERV);
		_ip = ip;
		_password = password;
		this->serverSocket = 0;
	};

Server::~Server() {
	std::cout << "Server destructor." << std::endl;
	delete _executor;
}

/* MEMBER FUNCTIONS*/

void Server::init() {
	this->serverSocket = socket(_address_family, _type, 0);
	if (this->serverSocket == -1)
		throw createSocketError();
	std::cout << "INIT OK" << std::endl;
	_handleMultiplesConnection();
}

void Server::socBind() {
	_hint.sin_family = _address_family;
	_hint.sin_port = htons(_port);
	_hint.sin_addr.s_addr = INADDR_ANY;
	//inet_pton(this->address_family, this->ip, &(this->hint.sin_addr));
	//this->hint.sin_addr = inet_addr(this->ip);				// inutile ?
	if (bind(this->serverSocket, (sockaddr*)&(_hint), sizeof(_hint)) == -1)
		throw bindSocketError();
	std::cout << "Bind ok, listener on port " << _port << std::endl;
}

void Server::mark() {
	if (listen(this->serverSocket, SOMAXCONN) < 0)
		throw markSocketError();
	std::cout << "MARK OK" << std::endl;
}



int Server::awaitForConnection() {
	socklen_t clientsize = sizeof(_client);
	memset(_host, 0, NI_MAXHOST);
	memset(_service, 0, NI_MAXSERV);

	int clientSocket = accept(this->serverSocket, (sockaddr*)(&_client), &clientsize);
	if (clientSocket == -1) {
		std::cerr << "Problem with the client connecting" << std::endl;
		return -4;
	}
	this->_logConnection();
	return clientSocket;
}

void sigHandler(int signal)
{
	if (signal == SIGINT)
	{
		progOver = true;
		std::cerr << "over" << std::endl;
	}
}

void Server::handle() {

	struct sigaction sigStruct;
	sigStruct.sa_handler = sigHandler;
	sigemptyset(&sigStruct.sa_mask);
	sigStruct.sa_flags = 0;
	sigaction(SIGINT, &sigStruct, NULL);

	_hintlen = sizeof(_hint);
	int i = 0, j = 0, activity = 0, valread = 0;
	char tmpBuff[4096];
	while (!progOver) {
		//Clear buffer & socket set
		memset(tmpBuff, 0, 4096);
		FD_ZERO(&_readfds);
		FD_SET(this->serverSocket, &_readfds);
		//add child sockets to set
		for (i = 0; i < client_socket.size(); i++) {   
			//if valid socket descriptor then add to read list
			if(client_socket[i] > 0)
				FD_SET(client_socket[i] , &_readfds);
		}
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely  
		activity = select(FD_SETSIZE, &_readfds , NULL , NULL , NULL);
		if (progOver)
		{
			for (std::vector<int>::iterator it = client_socket.begin(); it != client_socket.end(); it++)
				if(*it)
					close(*it);
			throw sigintReceived();
		}
		if ((activity < 0) && (errno != EINTR))
			std::cerr << "Error: select()" << std::endl;
		//If something happened on the master socket, then its an incoming connection else its some IO operation on some other socket 
		_acceptIncomingConnection();
		for (i = 0; i < client_socket.size(); i++) {
			_executor->setUserPtr(getUserBySocket(client_socket[i]));
			if (FD_ISSET(client_socket[i] , &_readfds)) {
				//Check if it was for closing , and also read the incoming message  
				if ((valread = recv(client_socket[i] , tmpBuff, 4096, 0)) == 0)
					_handleDisconnection(i, client_socket[i]);
				else {
					for (j = 0; _executor->getUserPtr()->buffer[j] != '\0'; j++);
					strcpy(_executor->getUserPtr()->buffer + j, tmpBuff);
					if (_executor->getUserPtr()->findNl(_executor->getUserPtr()->buffer)) {
						std::cout << _executor->getUserPtr()->buffer << std::flush;
						_executor->parseBuffer(_executor->getUserPtr()->buffer);
						memset(_executor->getUserPtr()->buffer, 0, 4096);
						_executor->execOPs();
					}
				}
			}
		}
	}
}


void Server::errorMsg(std::string reason, int clientSocket) {
	std::string msg = "ERROR: ";
	msg += reason;
	send(clientSocket, msg.c_str(), msg.size(), 0);
	for (std::vector<int>::iterator it = client_socket.begin(); it != client_socket.end(); it++)
	{
		if ((*it) == clientSocket) {
			client_socket.erase(it);
			break;
		}
	}
	for (std::list<User>::iterator ite = users.begin(); ite != users.end(); ite++) {
		if((*ite).getSocket() == clientSocket) {
			users.erase(ite);
			break;
		}
	}
	close(clientSocket);
}

void Server::cleanAnUser(int userSocket) {
	for (std::vector<int>::iterator it = client_socket.begin(); it != client_socket.end(); it++) {
		if ((*it) == userSocket) {
			client_socket.erase(it);
			break;
		}
	}
	for (std::list<User>::iterator it = users.begin(); it != users.end(); it++) {
		if ((*it).getSocket() == userSocket) {
			users.erase(it);
			break;
		}
	}
	close(userSocket);
}
/* PRIVATE FUNCTIONS */

void Server::_logConnection() {
	int result = getnameinfo((sockaddr*)&(_client), sizeof(_client), _host, NI_MAXHOST, _service, NI_MAXSERV, 0);
	if (result)
		std::cout << _host << " connected on " << _service << std::endl;
	else {
		inet_ntop(AF_INET, &(_client.sin_addr), _host, NI_MAXHOST);
		std::cout << _host << " connected on " << ntohs(_client.sin_port) << std::endl;
	}
}

void Server::_handleMultiplesConnection() {
	int opt = 1;
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
		throw setSockOptError();
}

void Server::_acceptIncomingConnection() {
	int new_socket = 0;
	if (FD_ISSET(this->serverSocket, &_readfds)) {
		if ((new_socket = accept(this->serverSocket, (struct sockaddr *)&_hint, (socklen_t*)&_hintlen)) < 0)
		{
			pr("asdasd");
			throw acceptSocketError();
		}
		printf("New connection, socket fd is %d, ip is : %s, port : %d \n", new_socket, inet_ntoa(_hint.sin_addr), ntohs(_hint.sin_port));
		client_socket.push_back(new_socket);
		User tmp(new_socket);
		users.push_back(tmp);
		std::cout << "Adding to list of sockets as " << client_socket.size() -1 << std::endl << std::endl;
	}
}

void Server::_handleDisconnection(int i, int sd) {
	getpeername(sd, (struct sockaddr*)&_hint, (socklen_t*)&_hintlen);
	printf("Host disconnected, ip %s, port %d \n", inet_ntoa(_hint.sin_addr), ntohs(_hint.sin_port));   
	//Close the socket and mark as 0 in list for reuse  
	close(sd);
	client_socket.erase(client_socket.begin() + i);
	for (std::list<User>::iterator it = users.begin(); it != users.end(); it++)
		if ((*it).getSocket() == sd) {
			users.erase(it);
			break;
		}
}

/* GETTERS & SETTERS */

void Server::setServerSocket(int servSock) {
	this->serverSocket = servSock;
}

User *Server::getUserBySocket(int socket) {
	
	for (std::list<User>::iterator it = users.begin(); it != users.end(); it++) {
		if ((*it).getSocket() == socket)
			return &(*it);
	}
	return NULL;
}

User *Server::getUserByUsername(std::string username) {
	
	for (std::list<User>::iterator it = users.begin(); it != users.end(); it++) {
		if ((*it).getUsername() == username)
			return &(*it);
	}
	return NULL;
}

std::string Server::getPassword() {
	return _password;
}