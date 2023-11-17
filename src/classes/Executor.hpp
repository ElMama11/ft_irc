#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <map>
# include <vector>
# include <string>
# include <sstream>
# include <sys/socket.h>
# include "User.hpp"
# include "ServerReply.hpp"
# include "Server.hpp"
# include "Channel.hpp"

class User;
class Server;
class Executor;
typedef void (Executor::*func)(std::string content);
typedef std::map<std::string, func> mapFunc;

struct OP {
	std::string	type;
	std::string content;
};

class Executor {

	private:
		void _cap(std::string content);
		void _nick(std::string content);
		void _user(std::string content);
		void _quit(std::string content);
		void _join(std::string content);
		void _pass(std::string content);
		void _mode(std::string content);
		void _privmsg(std::string content);

		void _createChannel(std::string content);

		mapFunc _mapping;
		Server *_server;
		User *_userPtr;
		std::vector<Channel> _channels;

	public:
		std::vector<OP>	ops;

		Executor(Server *ptr);
		~Executor(void);
		void parseBuffer(std::string content);
		void execOPs(void);
		Channel	*getChannelByName(std::string channelName);

		bool isChannel(std::string channel);
		bool isHash(std::string content);
		std::string	nextWord(std::string::size_type i, std::string content);
		bool	isDigit(std::string content);

		/* GETTERS & SETTERS*/
		void setUserPtr(User *ptr);
		User *getUserPtr();

};

#endif