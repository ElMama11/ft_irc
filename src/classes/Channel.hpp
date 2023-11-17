#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "User.hpp"

class	Channel
{
	private:
		bool					_topicRestrictionForOp;
		bool					_inviteOnly;
		size_t					_userLimits;
		std::string				_topic;
		std::string				_name;
		std::string				_pass;
		std::vector<User *>		_user;
		std::vector<User * >	_op;

	public:
		Channel(std::string chanName, User *userPtr);
		~Channel(void);

		bool			getTopicRestrictionForOp(void);
		void			setTopicRestrictionForOp(bool b);

		bool			getInviteOnly(void);
		void			setInviteOnly(bool b);

		unsigned int	getUserLimits(void);
		void			setUserLimits(unsigned int limit);
		
		unsigned int	countUsersInChannel();

		std::string		getName(void);
		void			setName(std::string name);

		std::string		getTopic(void);
		void			setTopic(std::string topic);

		std::string		getPass(void);
		void			setPass(std::string pass);

		User			*getUser(int sock);
		void			addUser(User *user, bool op);
		void			delUser(User *user);
		User			*getUserByNickname(std::string nickName);
		User			*getUserByUsername(std::string userName);
		unsigned int	totalUser(void);
		std::string		getAllUsersForNameReply();

		std::string		getNicknameWithPrefix(User *user);
		bool			isOp(User *user);
		bool			isUserByNickname(std::string nickname);

};

#endif