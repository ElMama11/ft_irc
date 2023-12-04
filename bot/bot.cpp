#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sstream>
#include <cctype>
#include <signal.h>

struct addrinfo	*res = NULL;
bool			progOver = false;

int	parsing(int ac, char **av)
{
	if (ac != 4)
	{
		std::cerr << "Error :format <port> <password> <channel>" << std::endl;
		return (-1);
	}
	
	std::string port(av[1]);
	std::string password(av[2]);
	std::string channel(av[3]);

	if (port == "" || password == "" || channel == "")
	{
		std::cerr << "Error :format <port> <password> <channel>" << std::endl;
		return (-1);
	}
	 for (size_t i = 0; i < port.length(); ++i)
        if (!isdigit(port[i]))
		{
			std::cerr << "Invalid port number" << std::endl;
            return (-1);
		}
	return (0);
}




int	createSocket(char **av)
{
	std::string port(av[1]);
	std::string password(av[2]);
	std::string channel(av[3]);

	struct addrinfo    hints;
  
    int                connectSocket;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo("localhost", av[1], &hints, &res))
    {
        std::cerr << "Can't get addr\n";
		freeaddrinfo(res);
        return -1;
    }
    connectSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (connectSocket == -1)
    {
        std::cerr << "Can't do socket\n";
        freeaddrinfo(res);
        return -1;
    }
    if (connect(connectSocket, res->ai_addr, (int)res->ai_addrlen) == SO_ERROR) {
        std::cerr << "Can't connect to server\n";
        close(connectSocket);
        freeaddrinfo(res);
        connectSocket = -1;
    }
    return connectSocket;
}




void	play(int socket, std::string channel)
{
	std::string	msg = "";
	std::string nl = "PRIVMSG " + channel + " :\n";
	

	for (int i = 0; i < 5; i++)
	{
		for (int i = 0; i < 16; i++)
			send(socket, nl.c_str(), nl.size(), 0);

		msg = "PRIVMSG " + channel + " :     ooo    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    ooooo   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     ooo    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :      o     \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    ooo     \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :   o  o     \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :  o   o     \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :      o     \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :      o     \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :      oo    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :      o o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :      o  oo \n";
		send(socket, msg.c_str(), msg.size(), 0);
		usleep(400000);

		for (int n = 0; n < 16 ; n++)
			send(socket, nl.c_str(), nl.size(), 0);

		msg = "PRIVMSG " + channel + " :    ooo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :   ooooo \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    ooo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     oo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		usleep(400000);

		for (int n = 0; n < 16; n++)
			send(socket, nl.c_str(), nl.size(), 0);

		msg = "PRIVMSG " + channel + " :   ooo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :  ooooo \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :   ooo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    oo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    o o \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :   oo   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :  o o   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " : o  oo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		usleep(400000);

		for (int n = 0; n < 16; n++)
			send(socket, nl.c_str(), nl.size(), 0);

		msg = "PRIVMSG " + channel + " :    ooo   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :   ooooo  \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :    ooo   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     o    \n";
		send(socket, msg.c_str(), msg.size(), 0);
		msg = "PRIVMSG " + channel + " :     oo   \n";
		send(socket, msg.c_str(), msg.size(), 0);
		usleep(400000);
	}
}

void sigHandler(int signal)
{
	if (signal == SIGINT)
		progOver = true;
}

void	bot(int socket, char **av)
{
	struct sigaction sigStruct;
	sigStruct.sa_handler = sigHandler;
	sigemptyset(&sigStruct.sa_mask);
	sigStruct.sa_flags = 0;
	sigaction(SIGINT, &sigStruct, NULL);
	std::string password(av[2]);
	std::string channel(av[3]);
	char	buff[4096];
	std::string tmp = "";

	std::string msg = "PASS " + password + "\n";
	std::cout << msg << std::endl;
	send(socket, msg.c_str(), msg.size(), 0);
	msg = "NICK bot\n";
	std::cout << msg << std::endl;
	send(socket, msg.c_str(), msg.size(), 0);
	msg = "USER Bot\n";
	send(socket, msg.c_str(), msg.size(), 0);
	msg = "JOIN " + channel + "\n";
	send(socket, msg.c_str(), msg.size(), 0);
	sleep(1);
	while (!progOver)
	{
		memset(buff, 0, 4096);
		if (recv(socket, buff, 4096, 0) == 0) {
			return ;
		}
			
		std::cout << "-- " << buff << std::endl;
		if (progOver || strcmp(buff, ":ft_irc 433 * bot :Nickname is already in use\r\n") == 0)
		{
			msg = "QUIT\n";
			send(socket, msg.c_str(), msg.size(), 0);
			close(socket);
			freeaddrinfo(res);
			return ;
		}
		std::istringstream iss(buff);
		while (iss >> tmp)
		{
			if (tmp == "PLAY")
				play(socket, channel);
		}
	}
}


int	main(int ac, char **av)
{
	int	socket = 0;
	if (parsing(ac, av) == -1)
		return (-1);
	socket = createSocket(av);
	if (av == 0)
		return (-1);
	else
		bot(socket, av);
	return (0);
}