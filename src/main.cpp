#include "classes/Socket.hpp"

int main(int ac, char **av) {

	if (ac != 3) {
		std::cerr << "USAGE : ./ircserv <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}
	MySocket *master_socket = new MySocket("0.0.0.0", atoi(av[1]), AF_INET, SOCK_STREAM);

	try {
		master_socket->init();
		master_socket->socBind();
		master_socket->mark();
		master_socket->handle();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	close(master_socket->socfd);
	delete master_socket;
	return 0;
}