#include "Executor.hpp"

Executor::Executor(Server *ptr) {
	this->_server = ptr;
	this->_userPtr = NULL;
	this->_mapping["CAP"] = &Executor::_cap;
	this->_mapping["NICK"] = &Executor::_nick;
	this->_mapping["USER"] = &Executor::_user;
	this->_mapping["QUIT"] = &Executor::_quit;
	return ;
}

Executor::~Executor(void) {
	return ;
}

void Executor::execOPs(void) {
	size_t size = this->ops.size();
	for (int i = 0; i < size; i++) {
		func f = _mapping[ops[i].type];
		(this->*f)(ops[i].content);		
	}
}

void Executor::parseBuffer(std::string content) {
	std::istringstream iss(content);
	std::string tmp;

	if (!(this->ops.empty())) {
		this->ops.clear();
	}

	while (std::getline(iss, tmp, '\n')) {
		int splitter = tmp.find(' ');
		std::string type = tmp.substr(0, splitter);
		OP op = {
			.type = tmp.substr(0, splitter),
			.content = tmp.substr(splitter+1, tmp.size())
		};
		this->ops.push_back(op);
	}
}

void Executor::setUserPtr(User *ptr) {
	this->_userPtr = ptr;
}


void Executor::_cap(std::string content) {
	return ;
}

void Executor::_nick(std::string content) {
	_userPtr->setNickname(content);

}

void Executor::_quit(std::string content) {
	std::cout << "QUIT: USER SOCKET: " << this->_userPtr->getSocket() << std::endl;
}

void Executor::_user(std::string content) {
	std::istringstream iss(content);
	std::string tmp;
	std::vector<std::string> params;
	while (std::getline(iss, tmp, ' '))
		params.push_back(tmp);
	if (params.size() > 0)
		this->_userPtr->setUsername(params[0]);
	if (params.size() >= 4)
		this->_userPtr->setRealname(params[3]);
	std::string msg;
	msg += "001 ";
	msg += _userPtr->getNickname();
	msg += " :Welcome to the irc Network, ";
	msg += _userPtr->getNickname();
	msg += "\n";
	std::cout << msg.c_str() << std::endl;
	send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
}