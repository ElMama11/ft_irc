#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>

int main() {
	// Create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listebing == -1) {
		std::cerr << "can't create a socket" << std::endl;
		return -1;
	}

	// Bind the socket to a IP/port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	inet_pton
	// Mark the socket for listening in
	// Accept a call
	// Close the listening socket
	// While receiving- display message, echo message
	// Close socket

	return 0;
}