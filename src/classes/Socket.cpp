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

int MySocket::soc_bind() {
	this->hint.sin_family = this->address_family;
	this->hint.sin_port = htons(this->port);
	this->hint.sin_addr.s_addr = INADDR_ANY;
	//inet_pton(this->address_family, this->ip, &(this->hint.sin_addr));
	//this->hint.sin_addr = inet_addr(this->ip);				// inutile ?
	if (bind(this->socfd, (sockaddr*)&(this->hint), sizeof(this->hint)) == -1) {
		std::cerr << "Can't bind to IP/port." << std::endl;
		return -2;
	}
	std::cout << "Bind ok, listener on port " << this->port << std::endl;
	return 0;
}

int MySocket::mark() {
	if (listen(this->socfd, SOMAXCONN) < 0) {
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

void MySocket::handle() {
	fd_set readfds;
	int max_sd;
	int client_socket[30];
	int i = 0, sd = 0, activity = 0, new_socket = 0, hintlen = sizeof(this->hint), valread = 0;
	//initialise all client_socket[] to 0 so not checked  
	for (i = 0; i < MAX_CLIENTS; i++) {
		client_socket[i] = 0;
	}
	while (true) {
		//Clear buffer & socket set
		memset(this->buffer, 0, 4096);
		FD_ZERO(&readfds);
		//add master socket to set
		FD_SET(this->socfd, &readfds);
		max_sd = this->socfd;
		//add child sockets to set
		for (i = 0; i < MAX_CLIENTS; i++)
		{   
			//socket descriptor
			sd = client_socket[i];
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
		//wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely  
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
		if ((activity < 0) && (errno != EINTR))
			printf("select error");
		//If something happened on the master socket, then its an incoming connection
		if (FD_ISSET(this->socfd, &readfds)) {
			if ((new_socket = accept(this->socfd, (struct sockaddr *)&hint, (socklen_t*)&hintlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(this->hint.sin_addr) , ntohs(this->hint.sin_port));
			//add new socket to array of sockets
			for (i = 0; i < MAX_CLIENTS; i++) {
				//if position is empty
				if(client_socket[i] == 0) {
					client_socket[i] = new_socket;   
					printf("Adding to list of sockets as %d\n", i);
					break;
				}
			}
		}
		//else its some IO operation on some other socket 
		for (i = 0; i < MAX_CLIENTS; i++) {
			sd = client_socket[i];
			if (FD_ISSET(sd , &readfds)) {
				//Check if it was for closing , and also read the incoming message  
				if ((valread = read( sd , buffer, 1024)) == 0) {
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&this->hint, (socklen_t*)&hintlen);
					printf("Host disconnected , ip %s , port %d \n", inet_ntoa(this->hint.sin_addr), ntohs(this->hint.sin_port));   
					//Close the socket and mark as 0 in list for reuse  
					close(sd);
					client_socket[i] = 0;
				}   
				//Echo back the message that came in  
				else {
					//set the string terminating NULL byte on the end of the data read  
					buffer[valread] = '\0';   
					send(sd, buffer, strlen(buffer), 0);
					std::cout << buffer << std::endl;
				}
			}
		}
	}
}

void MySocket::set_socfd(int socfd) {
	this->socfd = socfd;
}

/* PRIVATE FUNCTIONS */

void MySocket::_log_connection() {
	int result = getnameinfo((sockaddr*)&(this->client), sizeof(this->client), this->host, NI_MAXHOST, this->service, NI_MAXSERV, 0);
	if (result)
		std::cout << this->host << " connected on " << this->service << std::endl;
	else {
		inet_ntop(AF_INET, &(this->client.sin_addr), this->host, NI_MAXHOST);
		std::cout << this->host << " connected on " << ntohs(this->client.sin_port) << std::endl;
	}
}