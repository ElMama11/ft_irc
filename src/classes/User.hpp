#ifndef USER_HPP
# define USER_HPP

# include <string>


class User {

	private :
		int			_socket;
		std::string	_nickname;
		std::string	_username;

	public :
		User();
		~User();

		/* SETTERS */
		void setSocket(int socket);
		void setNickname(std::string nickname);
		void setUsername(std::string username);
	
};
#endif