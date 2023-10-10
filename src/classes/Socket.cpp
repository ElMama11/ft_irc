#include "Socket.hpp"

MySocket::MySocket(const char *ip, int port, int address_family, int type) {
		std::cout << "MySocket constructor." << std::endl;
		memset(this->buffer, 0, 4096);
		this->socfd = 0;
		this->ip = ip;
		this->port = port;
		this->address_family = address_family;
		this->type = type;
	};

MySocket::~MySocket() {
	std::cout << "MySocket destructor." << std::endl;
}

/* MEMBER FUNCTIONS*/

int MySocket::init() {
	this->socfd = socket(this->address_family, this->type, 0);
	if (this->socfd == -1) {
		std::cerr << "can't create a socket" << std::endl;
		return -1;
	}
	std::cout << "INIT OK" << std::endl;
	return 0;
}

int MySocket::mark() {
	if (listen(this->socfd, SOMAXCONN) == -1) {
		std::cerr << "Can't listen." << std::endl;
		return -3;
	}
	std::cout << "MARK OK" << std::endl;
	return 0;
}

int MySocket::await_for_connection() {
	socklen_t clientsize = sizeof(client);
	memset(this->host, 0, NI_MAXHOST);
	memset(this->service, 0, NI_MAXSERV);

	int clientSocket = accept(this->socfd, (sockaddr*)(&this->client), &clientsize);
	if (clientSocket == -1) {
		std::cerr << "Problem with the client connecting" << std::endl;
		return -4;
	}
	this->_log_connection();
	return clientSocket;
}

int MySocket::soc_bind() {
	this->hint.sin_family = this->address_family;
	this->hint.sin_port = htons(this->port);
	//inet_pton(this->address_family, this->ip, &(this->hint.sin_addr));
	//this->hint.sin_addr = inet_addr(this->ip);				// inutile ?
	if (bind(this->socfd, (sockaddr*)&(this->hint), sizeof(this->hint)) == -1) {
		std::cerr << "Can't bind to IP/port." << std::endl;
		return -2;
	}
	std::cout << "BIND OK" << std::endl;
	return 0;
}

void MySocket::handle() {
	while (true) {
		//Clear buffer
		memset(this->buffer, 0, 4096);
		//Wait a message
		int bytesRecv = recv(this->socfd, this->buffer, 4096, 0);
		if (bytesRecv == -1) {
			std::cerr << "connection issue" << std::endl;
			break;
		}
		if (bytesRecv == 0) {
			std::cout << "client disconnected" << std::endl;
			break;
		}
		//Display message
		std::cout << "Received: " << "socfd=" << this->socfd << " : " << 0 << bytesRecv << std::endl;
		std::cout << "Received: " << std::string(buffer) << std::endl;
		//Resend message
		send(this->socfd, this->buffer, bytesRecv + 1, 0);
	}
}

void MySocket::set_socfd(int socfd) {
	this->socfd = socfd;
}