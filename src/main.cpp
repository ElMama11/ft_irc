#include "classes/Socket.hpp"

int main() {

	MySocket *master_socket = new MySocket("0.0.0.0", 54003, AF_INET, SOCK_STREAM);
	int opt = 1, i = 0;

	master_socket->init();
	if (setsockopt(master_socket->socfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		std::cerr << "Error on setsockopt" << std::endl;
		return -1;
	}
	master_socket->soc_bind();
	master_socket->mark();

	//int user_soc_fd = master_socket->await_for_connection(); //a mettre dans le while
	
	//close(master_socket->socfd);
	//master_socket->set_socfd(user_soc_fd);
	master_socket->handle();

	close(master_socket->socfd);
	delete master_socket;
	return 0;
}