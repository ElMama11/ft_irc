#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "User.hpp"

class	Channel
{
	private:
		bool						_inviteOnly;
		size_t						_userLimits;
		std::string					_topic;
		std::string					_name;
		std::string					_pass;
		std::vector<User *>			_user;
		std::vector<User * >			_op;

	public:
		Channel(std::string chanName, User *userPtr);
		~Channel(void);

		bool		getInviteOnly(void);
		void		setInviteOnly(bool b);

		size_t		getUserLimits(void);
		void		setUserLimits(size_t limit);

		std::string	getName(void);
		void		setName(std::string name);

		std::string	getTopic(void);
		void		setTopic(std::string topic);

		std::string	getPass(void);
		void		setPass(std::string pass);

		User		*getUser(int sock);
		void		addUser(User *user, bool op);
		void		delUser(User *user);
		User		*getUserByNickname(std::string nickName);
		User		*getUserByUsername(std::string userName);
		size_t		totalUser(void);

};

#endif