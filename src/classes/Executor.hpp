#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <map>
# include <vector>
# include <string>
# include <sstream>
# include <sys/socket.h>
# include "User.hpp"
# include "ServerReply.hpp"

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

		mapFunc _mapping;
		Server *_server;
		User *_userPtr;

	public:
		Executor(Server *ptr);
		~Executor(void);
		void parseBuffer(std::string content);
		void execOPs(void);
		std::vector<OP>	ops;

		/* GETTERS & SETTERS*/
		void setUserPtr(User *ptr);
		User *getUserPtr();
};

#endif