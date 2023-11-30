# define SERV_NAME "ft_irc"
# define SERV_VERSION "Beta"

# define OPER_NAME "admin"
# define OPER_PASS "admin"

#define RPL_INVITE_RCV(client, channelName, targetName) ":" + client->getNickname() + " INVITE " + targetName + " :" + channelName + "\n"
#define RPL_JOIN(nickname, channelName) ":" + nickname + " JOIN :" + channelName + "\n"
#define RPL_PRIVMSG(client, targetName, message) ":" + client->getNickname() + "!" + client->getUsername() + "@localhost PRIVMSG " + targetName + " :" + message + "\n"
#define RPL_KICK(senderNick, channelName, targetNick, reason) ":" + senderNick + " KICK " + channelName + " " + targetNick + " " + reason + "\n"
#define RPL_MODE(user, channelName, message, targetName) ":" + user->getNickname() + "!~" + user->getUsername() + "@" SERV_NAME " MODE " + channelName + " " + message + " " + targetName + "\n"

/* 001 */ #define RPL_WELCOME(client) ":IRC 001 " + client->getNickname() + " :Welcome to the IRC network, " + client->getNickname() + "!" + "\n"
/* 002 */ #define RPL_YOURHOST(client) ":IRC 002 " + client->getNickname() + " :Your host is ft_irc, running beta" + "\n"
/* 003 */ #define RPL_CREATED(client, date) ":IRC 003 " + client.getNickname() + " :This server was created " + date + "\n"
/* 005 */ #define RPL_ISUPPORT(client) ":IRC 005 " + client.getNickname() + " TARGMAX=INVITE:1,PART:1,JOIN:1,KICK:1,PRIVMSG:1,WHOIS:1 CHANTYPES=# :are supported by this server" + "\n"

/* 302 */ #define NICK(oldNickname, newNickname) ":" + oldNickname + " NICK :" + newNickname + "\n"
/* 315 */ #define RPL_ENDOFWHO(client) ":" SERV_NAME " 315 " + client.getNickname() + " :End of WHO list" + "\n"
/* 324 */ #define RPL_CHANNELMODEIS(client, channel, mode) ":" SERV_NAME " 324 " + client->getNickname() + " " + channel + " " + mode + "\n"
/* 329 */ #define RPL_CREATIONTIME(client, channel, theTime) ":" SERV_NAME " 329 " + client->getNickname() + " " + channel + " " + theTime + "\n"
/* 331 */ #define RPL_NOTOPIC(client, channelName) ":" SERV_NAME " 331 " + client->getNickname() + " " + channelName + " :No topic is set" + "\n"
/* 332 */ #define RPL_TOPIC(client, channelName, topicText) ":" SERV_NAME " 332 " + client->getNickname() + " " + channelName + " " + topicText + "\n"
/* 341 */ #define RPL_INVITING(channelName, targetName) ":" SERV_NAME " 341 " + channelName + " " + targetName + "\n"
/* 353 */ #define RPL_NAMERPLY(client, channel, name) ":" SERV_NAME " 353 " + client->getNickname() + " @ " + channel.getName() + " :" + client->getNickname() + " " + name + "\n"
/* 353 */ #define RPL_NAMERPLYCREATECHANNEL(client, channel, name) ":" SERV_NAME " 353 " + client->getNickname() + " @ " + channel.getName() + " :" + name + "\n"
/* 366 */ #define RPL_ENDOFNAMES(channelName) ":" SERV_NAME " 366 " + channelName + " :End of /NAMES list." + "\n"
/* 401 */ #define ERR_NOSUCHNICK(client, targetName) ":" SERV_NAME " 401 " + client->getNickname() + " " + targetName + " :No such nick/channel" + "\n"
/* 401 */ #define ERR_NOSUCHNICKCHAN(client, channelName, targetName, command) ":" + client->getNickname() + " PRIVMSG " + channelName + " :Error " + command + ": No such nick (" + targetName + ")" + "\n"
/* 403 */ #define ERR_NOSUCHCHANNEL(client, channelName) ":" SERV_NAME " 403 " + client->getNickname() + " " + channelName + " :No such channel" + "\n"

/* 404 */ #define ERR_CANNOTSENDTOCHAN(client, channelName) ":" SERV_NAME " 404 " + client->getNickname() + " " + channelName + " :Cannot send to channel" + "\n"

/* 403 */ #define ERR_INVALIDCHANNEL(client, channelName) ":" SERV_NAME " 403 " + client->getNickname() + " " + channelName + " :Invalid channel name" + "\n"
/* 416 */ #define ERR_TOOMANYMATCHES(client) ":" SERV_NAME " 416 " + client.getNickname() + " :Command or message too long" + "\n"
/* 421 */ #define ERR_UNKNOWNCOMMAND(client, command) ":" SERV_NAME " 421 " + client->getNickname() + " " + command + " :Unknown command" + "\n"
/* 431 */ #define ERR_NONICKNAMEGIVEN(clientname) ":" SERV_NAME " 431 " + clientname + " :No nickname given" + "\n"
/* 432      */ #define ERR_ERRONEUSNICKNAME(nickname) ":" SERV_NAME " 432 Error " + nickname + " :Erroneous nickname" + "\n"
/* 433 */ #define ERR_NICKNAMEINUSE(clientName) ":" SERV_NAME " 433 * " + clientName + " :Nickname is already in use" + "\n"
/* 441 */ #define ERR_USERNOTINCHANNEL(client, targetName, channelName) ":" SERV_NAME " 441 " + client->getNickname() + " " + channelName + " " + targetName + " :They aren't on that channel" + "\n"
/* 442 */ #define ERR_NOTONCHANNEL(channelName) channelName + " :You're not on that channel" + "\n"
/* 443 */ #define ERR_USERONCHANNEL(targetName, channelName) ":" SERV_NAME " 443 " + targetName + " " + channelName + " :is already on channel" + "\n"
/* 451 */ #define ERR_NOTREGISTERED(client) ":" SERV_NAME " 451 " + client.getNickname() + " :You have not registered" + "\n"
/* 451 */ #define ERR_NOTREGISTERED_MSG(client, message) ":" SERV_NAME " 451 " + client.getNickname() + " :" + message + "\n"
/* 461  */ #define ERR_NEEDMOREPARAMS(client, command) ":" SERV_NAME " 461 " + client->getNickname() + " " + command + " :Not enough parameters" + "\n"
/* 462 */ #define ERR_TOOMANYPARAMS(command) ":" SERV_NAME " 462 " + command + " :Too many parameters" + "\n"
/* 463 		*/ #define ERR_ALREADYREGISTRED(clientName) ":" SERV_NAME " 463 " + clientName + " :Already registered" + "\n"

/* 464 */ #define ERR_PASSWDMISMATCH(client) ":" SERV_NAME " 464 " + client->getNickname() + " :Password Incorrect" + "\n"

/* 471 */ #define ERR_CHANNELISFULL(client, channelName) ":" SERV_NAME " 471 " + client->getNickname() + " " + channelName + " :Cannot join channel (+l) - channel is full, try again later" + "\n"
/* 472 */ #define ERR_UNKNOWNMODE(client, channelName, char) ":" SERV_NAME " 472 " + client->getNickname() + " " + channelName + " " + char + " :is unknown mode char to me" + "\n"
/* 473 */ #define ERR_INVITEONLYCHAN(client, channelName) ":" SERV_NAME " 473 " + client->getNickname() + " " + channelName + " :Cannot join channel (+i) - you must be invited" + "\n"
/* 475 */ #define ERR_BADCHANNELKEY(client, channelName) ":" SERV_NAME " 475 " + client->getNickname() + " " + channelName + " :Cannot join channel (+k) - bad key" + "\n"
/* 476 */ #define ERR_BADCHANNELMASK(channelName) ":" SERV_NAME " 476 " + channelName + " :Bad Channel Mask" + "\n"
/* 476 */ #define ERR_BADLIMIT(client, channelName, input) ":" SERV_NAME " 476 " + client.getNickname() + " " + channelName + " :Bad limit input (" + input + ")." + "\n"
/* 482 */ #define ERR_CHANOPRIVSNEEDED(client, channelName) ":" SERV_NAME " 482 " + client->getNickname() + " " + channelName + " :You're not channel operator" + "\n"

// # define RPL_WELCOME(user)  user + std::string(" Welcome to the ") SERV_NAME std::string(" Network, ") + user + "\n"
// # define RPL_YOURHOST       std::string("Your host is ") SERV_NAME std::string(", running version ") + SERV_VERSION + "\n"
// # define RPL_CREATED(time)  std::string("This server was created ") + time + "\n"
// # define RPL_MYINFO + "\n"
// # define RPL_ISUPPORT + "\n"
# define CHANMODES(modes) "CHANMODES=" + modes + "\n"
# define RPL_YOUREOPER      std::string("You are now an IRC operator") + "\n"

#define INVITE_MSG(clientName, targetName, chanName) ":" + clientName + " INVITE " + targetName + " " + chanName + "\n"