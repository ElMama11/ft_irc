#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <arpa/inet.h>
#include <cstring>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define MAX_CLIENTS 30

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
		void _log_connection();
};