#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>
# include <iostream>


class User {

	private :
		int			_socket;
		std::string	_nickname;
		std::string	_username;
		std::string _realname;

	public :
		User();
		User(std::vector<std::string> clientMsg);
		User(const User &cpy);
		User &operator=(const User &rhs);
		~User();

		/* FUNC */
		void parseRegistration(std::vector<std::string> clientMsg);
	
		/* GETTERS */
		int getSocket();
		std::string getNickname();
		std::string getUsername();
		std::string getRealname();
};
#endif