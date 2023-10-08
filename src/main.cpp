#include "classes/Socket.hpp"

int main() {

	MySocket *soc = new MySocket("0.0.0.0", 54003, AF_INET, SOCK_STREAM);
	
	soc->init();
	soc->soc_bind();
	soc->mark();

	int user_soc_fd = soc->await_for_connection();
	
	close(soc->socfd);
	
	soc->set_socfd(user_soc_fd);

	soc->handle();

	close(soc->socfd);

	delete soc;


	// // Create a socket
	// int listening = socket(AF_INET, SOCK_STREAM, 0);
	// if (listening == -1) {
	// 	std::cerr << "can't create a socket" << std::endl;
	// 	return -1;
	// }
	// // Bind the socket to a IP/port
	// sockaddr_in hint;
	// hint.sin_family = AF_INET;
	// hint.sin_port = htons(54000);
	// inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	// if (bind(listening , (sockaddr*)&hint, sizeof(hint)) == -1) {
	// 	std::cerr << "Can't bind to IP/port." << std::endl;
	// 	return -2;
	// }
	// // Mark the socket for listening in
	// if (listen(listening, SOMAXCONN) == -1) {
	// 	std::cerr << "Can't listen." << std::endl;
	// 	return -3;
	// }

	// std::cout << "BEFORE CALL" << std::endl;
	// // Accept a call
	// sockaddr_in client;
	// socklen_t clientsize = sizeof(client);

	// int clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
	// if (clientSocket == -1) {
	// 	std::cerr << "Problem with the client connecting" << std::endl;
	// 	return -4;
	// }
	// std::cout << "AFTER CALL" << std::endl;
	
	// // Close the listening socket
	// close(listening);
	
	// char host[NI_MAXHOST];
	// char service[NI_MAXSERV];
	// memset(host, 0, NI_MAXHOST);
	// memset(service, 0, NI_MAXSERV);

	// // While receiving- display message, echo message
	// int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
	// if (result) {
	// 	std::cout << host << " connected on " << service << std::endl;
	// }
	// else {
	// 	inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
	// 	std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	// }

	// char buf[4096];
	// while (true) {
	// 	//Clear buffer
	// 	memset(buf, 0, 4096);
	// 	//Wait a message
	// 	int bytesRecv = recv(clientSocket, buf, 4096, 0);
	// 	if (bytesRecv == -1) {
	// 		std::cerr << "connection issue" << std::endl;
	// 		break;
	// 	}
	// 	if (bytesRecv == 0) {
	// 		std::cout << "client disconnected" << std::endl;
	// 		break;
	// 	}
	// 	//Display message
	// 	std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	// 	//Resend message
	// 	send(clientSocket, buf, bytesRecv + 1, 0);
	// }
	// // Close socket
	// close(clientSocket);

	return 0;
}