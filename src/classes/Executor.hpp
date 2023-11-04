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


		mapFunc _mapping;
		Server *_server;
		User *_user_ptr;
	public:
		Executor(Server *ptr);
		~Executor(void);
		void parseBuffer(std::string content);
		void execOPs(void);
		void setUserPtr(User *ptr);
		std::vector<OP>	ops;
	protected:
};

#endif