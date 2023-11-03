#include "User.hpp"

User::User() {
}

User::User(int aSocket, std::string aNickname, std::string anUsername, std::string aRealname) : _socket(aSocket), _nickname(aNickname), _username(anUsername), _realname(aRealname) {
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

void parseRegistration(std::vector<std::string> clientMsg) {

}

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