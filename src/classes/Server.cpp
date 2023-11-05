#include "Server.hpp"

Server::Server(const char *ip, int port, int address_family, int type) {
		std::cout << "Server constructor." << std::endl;
		this->_executor = new Executor(this);
		memset(this->buffer, 0, 4096);
		this->socfd = 0;
		this->ip = ip;
		this->port = port;
		this->address_family = address_family;
		this->type = type;
	};

Server::~Server() {
	std::cout << "Server destructor." << std::endl;
}

/* MEMBER FUNCTIONS*/

void Server::init() {
	this->socfd = socket(this->address_family, this->type, 0);
	if (this->socfd == -1)
		throw createSocketError();
	std::cout << "INIT OK" << std::endl;
	_handleMultiplesConnection();
}

void Server::socBind() {
	this->hint.sin_family = this->address_family;
	this->hint.sin_port = htons(this->port);
	this->hint.sin_addr.s_addr = INADDR_ANY;
	//inet_pton(this->address_family, this->ip, &(this->hint.sin_addr));
	//this->hint.sin_addr = inet_addr(this->ip);				// inutile ?
	if (bind(this->socfd, (sockaddr*)&(this->hint), sizeof(this->hint)) == -1)
		throw bindSocketError();
	std::cout << "Bind ok, listener on port " << this->port << std::endl;
}

void Server::mark() {
	if (listen(this->socfd, SOMAXCONN) < 0)
		throw markSocketError();
	std::cout << "MARK OK" << std::endl;
}


User *Server::getUserBySocket(int socket) {
	std::vector<User>::iterator it = _users.begin();
	for (it; it < _users.end(); it++) {
		if ((*it).getSocket() == socket)
			return &(*it);
	}
	return NULL;
}

User *Server::getUserByUsername(std::string username) {
	std::vector<User>::iterator it = _users.begin();
	for (it; it < _users.end(); it++) {
		if ((*it).getUsername() == username)
			return &(*it);
	}
	return NULL;
}

int Server::awaitForConnection() {
	socklen_t clientsize = sizeof(client);
	memset(this->host, 0, NI_MAXHOST);
	memset(this->service, 0, NI_MAXSERV);

	int clientSocket = accept(this->socfd, (sockaddr*)(&this->client), &clientsize);
	if (clientSocket == -1) {
		std::cerr << "Problem with the client connecting" << std::endl;
		return -4;
	}
	this->_logConnection();
	return clientSocket;
}

void Server::handle() {
	int max_sd;
	this->_hintlen = sizeof(this->hint);
	int i = 0, sd = 0, activity = 0, valread = 0;
	while (true) {
		//Clear buffer & socket set
		memset(this->buffer, 0, 4096);
		FD_ZERO(&this->_readfds);
		//add master socket to set
		FD_SET(this->socfd, &this->_readfds);
		max_sd = this->socfd;
		//add child sockets to set
		for (i = 0; i < _client_socket.size(); i++) {   
			//socket descriptor
			sd = this->_client_socket[i];
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET(sd , &this->_readfds);
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely  
		activity = select(max_sd + 1 , &this->_readfds , NULL , NULL , NULL);
		if ((activity < 0) && (errno != EINTR))
			std::cerr << "Error: select()" << std::endl;
		//If something happened on the master socket, then its an incoming connection
		_acceptIncomingConnection();
		//else its some IO operation on some other socket 
		for (i = 0; i < _client_socket.size(); i++) {
			this->_executor->setUserPtr(getUserBySocket(_client_socket[i]));
			sd = this->_client_socket[i];
			if (FD_ISSET(sd , &this->_readfds)) {
				//Check if it was for closing , and also read the incoming message  
				if ((valread = recv(sd , buffer, 4096, 0)) == 0)
					_handleDisconnection(i, sd);
				//Echo back the message that came in  
				else {
					//set the string terminating NULL byte on the end of the data read
					buffer[valread] = '\0';   
					// send(sd, buffer, strlen(buffer), 0);
					std::cout << buffer;
					this->_executor->parseBuffer(buffer);
					_executor->execOPs();
				}
			}
		}
	}
}


void Server::setSocfd(int socfd) {
	this->socfd = socfd;
}

/* PRIVATE FUNCTIONS */

void Server::_logConnection() {
	int result = getnameinfo((sockaddr*)&(this->client), sizeof(this->client), this->host, NI_MAXHOST, this->service, NI_MAXSERV, 0);
	if (result)
		std::cout << this->host << " connected on " << this->service << std::endl;
	else {
		inet_ntop(AF_INET, &(this->client.sin_addr), this->host, NI_MAXHOST);
		std::cout << this->host << " connected on " << ntohs(this->client.sin_port) << std::endl;
	}
}

void Server::_handleMultiplesConnection() {
	int opt = 1;
	if (setsockopt(this->socfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
		throw setSockOptError();
}

void Server::_acceptIncomingConnection() {
	int i = 0, new_socket = 0;
	if (FD_ISSET(this->socfd, &this->_readfds)) {
		if ((new_socket = accept(this->socfd, (struct sockaddr *)&hint, (socklen_t*)&this->_hintlen)) < 0) 
			throw acceptSocketError();
		//inform user of socket number - used in send and receive commands
		printf("New connection, socket fd is %d, ip is : %s, port : %d \n", new_socket, inet_ntoa(this->hint.sin_addr), ntohs(this->hint.sin_port));
		
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
	//Somebody disconnected , get his details and print
	getpeername(sd, (struct sockaddr*)&this->hint, (socklen_t*)&this->_hintlen);
	printf("Host disconnected, ip %s, port %d \n", inet_ntoa(this->hint.sin_addr), ntohs(this->hint.sin_port));   
	//Close the socket and mark as 0 in list for reuse  
	close(sd);
	this->_client_socket.erase(this->_client_socket.begin() + i);
	this->_users[i].setSocket(0);
}