#include "Executor.hpp"
#include "Channel.hpp"
#include "../pr.cpp"
#include <limits.h>

Executor::Executor(Server *ptr) {
	this->_server = ptr;
	this->_userPtr = NULL;
	this->_mapping["CAP"] = &Executor::_cap;
	this->_mapping["NICK"] = &Executor::_nick;
	this->_mapping["USER"] = &Executor::_user;
	this->_mapping["QUIT"] = &Executor::_quit;
	this->_mapping["JOIN"] = &Executor::_join;
	this->_mapping["PASS"] = &Executor::_pass;
	this->_mapping["MODE"] = &Executor::_mode;
	//this->_mapping["PRIVMSG"] = &Executor::_privmsg;
	return ;
}

Executor::~Executor(void) {
	return ;
}

void Executor::execOPs(void) {
	size_t size = this->ops.size();
	std::string msg;
	for (int i = 0; i < size; i++) {
		if (_mapping.find(ops[i].type) == _mapping.end()) {
			msg = ERR_UNKNOWNCOMMAND(_userPtr, ops[i].type);
			send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
			break;
		}
		func f = _mapping[ops[i].type];
		(this->*f)(ops[i].content);	
		ops[i].type = "";
		ops[i].content = "";
	}
}

void Executor::parseBuffer(std::string content) {
	std::istringstream iss(content);
	std::string tmp;
	int pos = 0;

	if (!(this->ops.empty())) {
		this->ops.clear();
	}
	while (std::getline(iss, tmp, '\n')) {
		int splitter = tmp.find(' ');
		if (pos = tmp.find('\r'))
			tmp = tmp.substr(0, pos);
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
		return;
	while (std::getline(iss, tmp, ' '))
		params.push_back(tmp);
	if (params.size() > 0)
		this->_userPtr->setUsername(params[0]);
	if (params.size() >= 4)
		this->_userPtr->setRealname(params[3]);
	}
	std::string msg = RPL_WELCOME(_userPtr);
	send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
}

void Executor::_join(std::string content) {
	std::string msg;

	std::string firstword;
	std::istringstream iss(content);
	iss >> firstword;

	if (firstword.find('#') == -1 || firstword == "#") {
		msg = ERR_INVALIDCHANNEL(_userPtr, content);
		send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
		return ;
	}
	if (!isChannel(content)) {
		_createChannel(content);
	}
	else {
		Channel *chanToJoin = getChannelByName(content);
		if (chanToJoin == NULL) {
			std::cout << "ERROR CHANNEL DONT EXIST" << std::endl;
			return;
		}
		else if (chanToJoin->countUsersInChannel() >= chanToJoin->getUserLimits()) {
			std::cout << "PU DE PLACE" << std::endl;
			return;
		}
		else if (chanToJoin->getInviteOnly() == true && _userPtr->isInvited(content) == false) {
			std::cout << "LE CHAN EST EN INVIT ONLY" << std::endl;
			return;
		}
		else if (chanToJoin->getPass() != "") {
			int end, start = 0;
			start = content.find(" ");
			start++;
			end = start;
			while (content[end] != ' ' && content[end] != '\0')
				end++; 
			std::string userPass = content.substr(start, end - start);
			if (userPass != chanToJoin->getPass()) {
				std::cout << "NEED un mdp" << std::endl;
				return;
			}
		}
		else {
			std::string msg = RPL_JOIN(_userPtr->getNickname(), content);
			send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
			msg = RPL_TOPIC(_userPtr, content, chanToJoin->getTopic());
			send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
			msg += RPL_NAMERPLY(_userPtr, (*chanToJoin), chanToJoin->getAllUsersForNameReply());
			pr(msg);
			send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
			msg = RPL_ENDOFNAMES(chanToJoin->getName());
			send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
			// renvoyer la rpl a tout le monde
			char	*str;
			std::string everyone = chanToJoin->getAllUsersForNameReply();
			str = strtok(const_cast<char *>(everyone.c_str()), " ");
			while (str != NULL)
			{
				std::string name(str);
				pr(name);
				User *tmp;
				if (str[0] == '@')
					name = name.substr(1, name.length());
				tmp = chanToJoin->getUserByNickname(name);
				msg = RPL_JOIN(_userPtr->getNickname(), content);
				send(tmp->getSocket(), msg.c_str(), msg.size(), 0);
				str = strtok(0, " ");
			}
			chanToJoin->addUser(_userPtr, false);
		}	
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
		_userPtr->setNickname("placeholder");
		std::string msg = ERR_PASSWDMISMATCH(_userPtr);
		send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
		_server->errorMsg("ERROR :Closing Link: localhost (Bad Password)\n<connection gets terminated by the server>\n", _userPtr->getSocket());
	}
}

void Executor::_createChannel(std::string content) {
	Channel newChan(content, _userPtr);
	_channels.push_back(newChan);
	std::string msg = RPL_JOIN(_userPtr->getNickname(), content);
	send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
	msg = RPL_NAMERPLYCREATECHANNEL(_userPtr, newChan, newChan.getNicknameWithPrefix(_userPtr));
	send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
	msg = RPL_ENDOFNAMES(newChan.getName());
	send(_userPtr->getSocket(), msg.c_str(), msg.size(), 0);
	// for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	// 	pr((*it).getName());   		Delete channel quand y a plus personne
}

bool Executor::isChannel(std::string channel)
{
	for(std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		if (channel == (*it).getName())
			return (true);
	return (false);
}


bool Executor::isHash(std::string content)
{
	if (content[0] != '#')
		return (false);
	return (true);
}

std::string	Executor::nextWord(std::string::size_type i, std::string content)
{
	std::string::size_type start = i + 1;
	std::string::size_type end = i + 1;

	while (content[end] != ' ' && content[end] != 13 && content[end] != '\0')
		end++;
	return (content.substr(start, end - start));
}

bool	Executor::isDigit(std::string content)
{
	for (int i = 0; content[i] != '\0'; i++)
		if (!isdigit(content[i]))
			return (false);
	return (true);
}

void Executor::_mode(std::string content)
{
	std::string	arg;
	size_t		pos = content.find(' ');
	std::vector<Channel>::iterator it = _channels.begin();

	std::string	firstWord = content.substr(0, pos);
	pos = content.find('\r');
	content = content.substr(0, pos);
	while (it != _channels.end() && (*it).getName() != firstWord)
		it++;
	if (it == _channels.end())
	{
		std::cout << content << "---error, channel not found" << std::endl; // le channel n'existe pas
		return ;
	}
	else if ((*it).isOp(_userPtr) == false)
	{
		std::cout << "error, you not operator in this channel" << std::endl; // n'est pas operator du channel
		return ;
	}
	else
	{
		std::string::size_type start = 0;

		for (std::string::size_type i = 0; i <= content.length(); i++)
		{
			if (content[i] == ' ' || i == content.length())
			{
				arg = content.substr(start, i - start);
				if (arg == "+i")
					(*it).setInviteOnly(true);
				else if (arg == "-i")
					(*it).setInviteOnly(false);
				else if (arg == "+t")
					(*it).setTopicRestrictionForOp(true);
				else if (arg == "-t")
					(*it).setTopicRestrictionForOp(false);
				else if (arg == "+k")
					(*it).setPass(nextWord(i, content));
				else if (arg == "-k")
					(*it).setPass("");
				else if (arg == "+o")
				{
					std::string user = nextWord(i, content);

					if ((*it).isUserByNickname(user))
					{
						User	*tmp = (*it).getUserByNickname(user);
						(*it).delUser(tmp);
						(*it).addUser(tmp, true);
					}
					else
						std::cout << "error, user not found" << std::endl; // l'utilisateur cible n'est pas dans le channel
				}
				else if (arg == "-o")
				{
					User	*tmp = (*it).getUserByNickname(nextWord(i, content));
					if ((*it).isOp(tmp))
					{
						(*it).delUser(tmp);
						(*it).addUser(tmp, false);
					}
					else
						std::cout << "error, user not operator" << std::endl; // l'utilisateur cible n'est pas dans la liste des operator, faire la distinction avec son absence dans le channel ?
				}
				else if (arg == "+l")
				{
					if (isDigit(nextWord(i, content)))
						(*it).setUserLimits(static_cast<unsigned int>(std::atoi(nextWord(i, content).c_str())));
					else
						std::cout << "error, num only for user limit" << std::endl;
				}
				else if (arg == "-l")
					(*it).setUserLimits(UINT_MAX);
				start = i + 1;
			}
		}
	}
}
  
Channel *Executor::getChannelByName(std::string channelName) {
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		if (it->getName() == channelName)
			return &(*it);
	}
	return NULL;
}

 /* GETTERS & SETTERS */
void Executor::setUserPtr(User *ptr) {
	this->_userPtr = ptr;
}

User *Executor::getUserPtr() {
	return _userPtr;
}