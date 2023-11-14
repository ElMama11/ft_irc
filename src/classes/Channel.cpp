#include "Channel.hpp"

Channel::Channel(std::string chanName, User *userPtr): _name(chanName) {
	_inviteOnly = false;
	_userLimits = 999;
	_topic = "";
	_pass = "";
	addUser(userPtr, true);
}

Channel::~Channel(void) {
}

void		Channel::setInviteOnly(bool b)
{
	_inviteOnly = b;
}

bool		Channel::getInviteOnly(void)
{
	return (_inviteOnly);
}



void		Channel::setUserLimits(size_t limit)
{
	_userLimits = limit;
}

size_t		Channel::getUserLimits(void)
{
	return (_userLimits);
}



void		Channel::setName(std::string name)
{
	_name = name;
}

std::string	Channel::getName(void)
{
	return (_name);
}



void		Channel::setTopic(std::string topic)
{
	_topic = topic;
}

std::string	Channel::getTopic(void)
{
	return (_topic);
}



void		Channel::setPass(std::string pass)
{
	_pass = pass;
}

std::string	Channel::getPass(void)
{
	return (_pass);
}



void		Channel::addUser(User *user, bool op)
{
	if (op == true)
		_op.push_back(user);
	else
		_user.push_back(user);
}

void	Channel::delUser(User *user)
{
	std::vector<User *>::iterator it;

	it = _op.begin();
	while (it != _op.end())
	{
		if ((*it)->getSocket() == user->getSocket())
			_op.erase(it);
		it++;
	}

	it = _user.begin();
	while (it != _user.begin())
	{
		if ((*it)->getSocket() == user->getSocket())
			_user.erase(it);
		it++;
	}
}

User		*Channel::getUser(int sock)
{
	std::vector<User *>::iterator it;

	it = _op.begin();
	while (it != _op.end())
	{
		if ((*it)->getSocket() == sock)
			return (*it);
		it++;
	}

	it = _user.begin();
	while (it != _user.end())
	{
		if ((*it)->getSocket() == sock)
			return (*it);
		it++;
	}

	return (NULL);
}

User		*Channel::getUserByNickname(std::string nickName)
{
	std::vector<User *>::iterator it;

	it = _op.begin();
	while (it != _op.end())
	{
		if ((*it)->getNickname() == nickName)
			return (*it);
		it++;
	}

	it = _user.begin();
	while (it != _user.end())
	{
		if ((*it)->getNickname() == nickName)
			return (*it);
		it++;
	}

	return (NULL);
}

User		*Channel::getUserByUsername(std::string userName)
{
	std::vector<User *>::iterator it;

	it = _op.begin();
	while (it != _op.end())
	{
		if ((*it)->getUsername() == userName)
			return (*it);
		it++;
	}

	it = _user.begin();
	while (it != _user.end())
	{
		if ((*it)->getUsername() == userName)
			return (*it);
		it++;
	}

	return (NULL);
}

size_t		Channel::totalUser(void)
{
	size_t	nb = 0;
	std::vector<User *>::iterator it;

	it = _op.begin();
	while (it != _op.end())
	{
		it++;
		nb++;
	}

	it = _user.begin();
	while (it != _user.end())
	{
		it++;
		nb++;
	}

	return (nb);
}