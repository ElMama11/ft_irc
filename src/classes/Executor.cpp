#include "Executor.hpp"

Executor::Executor(Server *ptr) {
	this->_server = ptr;
	this->_userPtr = NULL;
	this->_mapping["CAP"] = &Executor::_cap;
	this->_mapping["NICK"] = &Executor::_nick;
	this->_mapping["USER"] = &Executor::_user;
	this->_mapping["QUIT"] = &Executor::_quit;
	this->_mapping["JOIN"] = &Executor::_join;
	this->_mapping["PASS"] = &Executor::_pass;
	return ;
}

Executor::~Executor(void) {
	return ;
}

void Executor::execOPs(void) {
	size_t size = this->ops.size();
	for (int i = 0; i < size; i++) {
		if (_mapping.find(ops[i].type) == _mapping.end())
			break;
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



void Executor::_cap(std::string content) {
	return ;
}

void Executor::_nick(std::string content) {
	_userPtr->setNickname(content);

}

void Executor::_quit(std::string content) {
	std::cout << "func _quit: USER SOCKET: " << this->_userPtr->getSocket() << std::endl;
}

void Executor::_user(std::string content) {
	std::istringstream iss(content);
	std::string tmp;
	std::vector<std::string> params;

	if (_userPtr->checkPassword == false) {
		_pass("");
		//envoyer la bonne erreur ?
		return;
	}
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
	send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
}

void Executor::_join(std::string content) {
	std::string msg;

	std::string firstword;
	std::istringstream iss(content);
	iss >> firstword;

	if (firstword.find('#') == -1 || firstword == "#") {
		std::cout << "error 403" << std::endl; //403 NICK CHAN :No such channel
		return ;
	}
	if (!isChannel(content)) {
		_createChannel(content);
	}
}

void Executor::_pass(std::string content) {
	if(content.find(13) != -1) {
		content.erase(content.find(13));
	}
	if (_server->getPassword().compare(content) == 0) {
		_userPtr->checkPassword = true;
		return ;
	}
	else {
		//std::cout << content << " && " << _server->getPassword().compare(content) <<  std::endl;
		std::string msg = ":ft_irc ";
		msg += "464 ";
		msg += _userPtr->getNickname();
		msg += " :Password Incorrect";
		send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
		_server->errorMsg(":Closing Link: localhost (Bad Password or no password supplied) <connection gets terminated by the server>\n", _userPtr->getSocket());
	}
}

void Executor::_createChannel(std::string content) {
	Channel newChan(content, _userPtr);
	_channels.push_back(newChan);
	std::string nickname = _userPtr->getNickname();
	std::string msg = RPL_JOIN(nickname, content);
	send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
	msg = RPL_NAMERPLY(nickname, )
}

 /* GETTERS & SETTERS */
void Executor::setUserPtr(User *ptr) {
	this->_userPtr = ptr;
}

User *Executor::getUserPtr() {
	return _userPtr;
}

bool Executor::isChannel(std::string channel)
{
	for(std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		if (channel == (*it).getName())
			return (true);
	return (false);
}
