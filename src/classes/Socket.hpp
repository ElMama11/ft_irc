#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <sys/types.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string>
# include <arpa/inet.h>
# include <cstring>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>


# define MAX_CLIENTS 30

// class SocketsManager {
// 	public:
// 		MySocket connection_handler;
// 		MySocket *socs;
// };

class MySocket {

	private:
		void _log_connection();
		int _handle_multiples_connection();

	public:
	
		MySocket(const char *ip, int port, int address_family, int type);
		~MySocket();

		void init();
		void mark();
		int await_for_connection();
		void soc_bind();
		void handle();
		void set_socfd(int socfd);

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

		
		/* ERROR CLASSES*/

		class bindSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Error: Can't bind socket to IP/port.");
				}
		};

		class createSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Error: Can't create a socket");
				}
		};

		class markSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Error: Can't set the socket on listening");
				}
		};
};

#endif