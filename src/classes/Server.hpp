#ifndef SERVER_HPP
# define SERVER_HPP

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
# include <map>
# include <list>
# include <vector>
# include "User.hpp"
# include "Executor.hpp"


# define MAX_CLIENTS 30

// class SocketsManager {
// 	public:
// 		Server connection_handler;
// 		Server *socs;
// };

class Server {

	private:
		void	_logConnection();
		void	_handleMultiplesConnection();
		void	_acceptIncomingConnection();
		void	_handleDisconnection(int i, int sd);

		fd_set	_readfds;
		std::vector<int> _client_socket;
		int		_hintlen;

		std::vector<User>	_users;
		Executor		*_executor;
		

	public:
	
		Server(const char *ip, int port, int address_family, int type);
		~Server();

		void	init();
		void	mark();
		int		awaitForConnection();
		void	socBind();
		void	handle();
		void	setSocfd(int socfd);

		User	*getUserBySocket(int socket);
		User	*getUserByUsername(std::string username);

		int			socfd;
		int			port;
		int			address_family;
		int			type;
		char		host[NI_MAXHOST];
		char		service[NI_MAXSERV];
		char		buffer[4096];
		const char	*ip;
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

		class setSockOptError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Error: setsockopt()");
				}
		};

		class acceptSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Error: accept()");
				}
		};
};

#endif