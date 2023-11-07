#include "Executor.hpp"

Executor::Executor(Server *ptr) {
	this->_server = ptr;
	this->_user_ptr = NULL;
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
	this->_user_ptr = ptr;
}


void Executor::_cap(std::string content) {
	return ;
}

void Executor::_nick(std::string content) {
	_user_ptr->setNickname(content);

}

void Executor::_quit(std::string content) {
	std::cout << "QUIT: USER SOCKET: " << this->_user_ptr->getSocket() << std::endl;
}

void Executor::_user(std::string content) {
	std::istringstream iss(content);
	std::string tmp;
	std::vector<std::string> params;
	while (std::getline(iss, tmp, ' '))
		params.push_back(tmp);
	if (params.size() > 0)
		this->_user_ptr->setUsername(params[0]);
	if (params.size() >= 4)
		this->_user_ptr->setRealname(params[3]);
	std::string msg = (":irc " + RPL_WELCOME + std::string(" ") + _user_ptr->getNickname() + "Welcome to the MamaIRC Network, " + _user_ptr->getNickname());
	send(_user_ptr->getSocket(), msg.c_str(), msg.size(), 0);
}