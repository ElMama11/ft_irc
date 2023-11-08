#include "Server.hpp"

Server::Server(const char *ip, int port, int address_family, int type) {
		std::cout << "Server constructor." << std::endl;
		_executor = new Executor(this);
		memset(_buffer, 0, 4096);
		this->serverSocket = 0;
		_ip = ip;
		_port = port;
		_address_family = address_family;
		_type = type;
	};

Server::~Server() {
	std::cout << "Server destructor." << std::endl;
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


User *Server::getUserBySocket(int socket) {
	
	for (std::vector<User>::iterator it = _users.begin(); it < _users.end(); it++) {
		if ((*it).getSocket() == socket)
			return &(*it);
	}
	return NULL;
}

User *Server::getUserByUsername(std::string username) {
	
	for (std::vector<User>::iterator it = _users.begin(); it < _users.end(); it++) {
		if ((*it).getUsername() == username)
			return &(*it);
	}
	return NULL;
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

void Server::handle() {

	this->_hintlen = sizeof(_hint);
	int i = 0, activity = 0, valread = 0;
	while (true) {
		//Clear buffer & socket set
		memset(_buffer, 0, 4096);
		FD_ZERO(&this->_readfds);
		FD_SET(this->serverSocket, &this->_readfds);
		//add child sockets to set
		for (i = 0; i < _client_socket.size(); i++) {   
			//if valid socket descriptor then add to read list
			if(this->_client_socket[i] > 0)
				FD_SET(this->_client_socket[i] , &this->_readfds);
		}
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely  
		activity = select(FD_SETSIZE, &this->_readfds , NULL , NULL , NULL);
		if ((activity < 0) && (errno != EINTR))
			std::cerr << "Error: select()" << std::endl;
		//If something happened on the master socket, then its an incoming connection else its some IO operation on some other socket 
		_acceptIncomingConnection();
		for (i = 0; i < _client_socket.size(); i++) {
			this->_executor->setUserPtr(getUserBySocket(_client_socket[i]));
			if (FD_ISSET(this->_client_socket[i] , &this->_readfds)) {
				//Check if it was for closing , and also read the incoming message  
				if ((valread = recv(this->_client_socket[i] , _buffer, 4096, 0)) == 0)
					_handleDisconnection(i, this->_client_socket[i]);
				else {
					_buffer[valread] = '\0';   
					std::cout << _buffer;
					this->_executor->parseBuffer(_buffer);
					_executor->execOPs();
				}
			}
		}
	}
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
	int i = 0, new_socket = 0;
	if (FD_ISSET(this->serverSocket, &this->_readfds)) {
		if ((new_socket = accept(this->serverSocket, (struct sockaddr *)&_hint, (socklen_t*)&this->_hintlen)) < 0) 
			throw acceptSocketError();
		printf("New connection, socket fd is %d, ip is : %s, port : %d \n", new_socket, inet_ntoa(_hint.sin_addr), ntohs(_hint.sin_port));
		this->_client_socket.push_back(new_socket);
		User tmp(new_socket);
		this->_users.push_back(tmp);
		std::cout << "Adding to list of sockets as " << this->_client_socket.size() -1 << std::endl << std::endl;
		//add new socket to array of sockets
		// for (i = 0; i < MAX_CLIENTS; i++) {
		// 	//if position is empty
		// 	if(this->_client_socket[i] == 0) {
		// 		this->_client_socket[i] = new_socket; 
		// 		User tmp(new_socket);
		// 		_users->push_back(tmp);
		// 		std::cout << "Adding to list of sockets as " << i << std::endl << std::endl;
		// 		break;
		// 	}
		// }
	}
}

void Server::_handleDisconnection(int i, int sd) {
	getpeername(sd, (struct sockaddr*)&_hint, (socklen_t*)&this->_hintlen);
	printf("Host disconnected, ip %s, port %d \n", inet_ntoa(_hint.sin_addr), ntohs(_hint.sin_port));   
	//Close the socket and mark as 0 in list for reuse  
	close(sd);
	this->_client_socket.erase(this->_client_socket.begin() + i);
	this->_users[i].setSocket(0);
}

/* SETTERS */

void Server::setServerSocket(int servSock) {
	this->serverSocket = servSock;
}