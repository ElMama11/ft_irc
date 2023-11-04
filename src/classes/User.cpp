#include "User.hpp"

User::User() {
}

User::User(int socket) : _socket(socket) {

}

User::User(const User &cpy) {
	*this = cpy;
}

User &User::operator=(const User &rhs) {
	if (this != &rhs) {
		_socket = rhs._socket;
		_nickname = rhs._nickname;
		_username = rhs._username;
		_realname = rhs._realname;
	}
	return *this;
}

User::~User() {
}

/* FUNC */

/* GETTERS */

int User::getSocket() {
	return _socket;
}

std::string User::getNickname() {
	return _nickname;
}

std::string User::getUsername() {
	return _username;
}

std::string User::getRealname() {
	return _realname;
}

/* SETTERS */

void User::setSocket(int socket) {
	_socket = socket;
}

void User::setNickname(std::string nickname) {
	_nickname = nickname;
}

void User::setUsername(std::string username) {
	_username = username;
}

void User::setRealname(std::string realname) {
	_realname = realname;
}