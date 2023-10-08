#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <arpa/inet.h>
#include <cstring>

// class SocketsManager {
// 	public:
// 		MySocket connection_handler;
// 		MySocket *socs;
// };

class MySocket {
	public:
	
		int socfd;
		int port;
		int address_family;
		int type;
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];
		char buffer[4096];
		const char *ip;
		sockaddr_in hint;
		sockaddr_in client;

		MySocket(const char *ip, int port, int address_family, int type);
		~MySocket();

		int init();
		int mark();
		int await_for_connection();
		int soc_bind();
		void handle();
		void set_socfd(int socfd);

	private:
		void _log_connection() {
			int result = getnameinfo((sockaddr*)&(this->client), sizeof(this->client), this->host, NI_MAXHOST, this->service, NI_MAXSERV, 0);
			if (result) {
				std::cout << this->host << " connected on " << this->service << std::endl;
			}
			else {
				inet_ntop(AF_INET, &(this->client.sin_addr), this->host, NI_MAXHOST);
				std::cout << this->host << " connected on " << ntohs(this->client.sin_port) << std::endl;
			}
	}
};