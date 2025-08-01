#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP


//might not use the numbers but for now i'm leaving them here
//command codes
# define	RPL_WELCOME				001
# define	RPL_NOTOPICSET			331
# define	RPL_TOPICSET			332

# define	RPL_NAMESMSG			353
# define	RPL_ENDNAMELIST			366
//error codes
# define	ERR_NOSUCHCHANNEL		403
# define	ERR_TOOMANYCHANNELS		405
# define	ERR_NOORIGIN			409
# define	ERR_NORECIPIENT			411
# define	ERR_NOTEXTTOSEND		412
# define	ERR_UNKNOWNCOMMAND		421
# define	ERR_NONICKNAMEGIVEN		431
# define	ERR_ERRONEUSENICKNAME	432
# define	ERR_NICKNAMEINUSE		433
# define	ERR_NOTREGISTERED		451
# define	ERR_NEEDMOREPARAMS		461
# define	ERR_ALREADYREGISTERED	462
# define	ERR_PASSMISMATCH		464
# define	ERR_CHANNELISFULL		471
# define	ERR_INVITEONLYCHAN		473
# define	ERR_BADCHANNELKEY		475
# define	ERR_BADCHANMASK			476

//Messages
inline	std::string	rplWelcome(std::string nickname){
	return (":localhost 001 " + nickname + " :Welcome to our IRC server, " + nickname + ":)\r\n");
}

inline	std::string	rplNamesMsg(std::string namesMsg){
	return (namesMsg + "\r\n");
}

inline	std::string	rplNoTopicSet(std::string nickname, std::string channelName){
	return (":localhost 331 " + nickname + " " + channelName + " :No topic is set\r\n");
}

inline	std::string	rplTopicSet(std::string nickname, std::string channelName, std::string topic){
	return (":localhost 332 " + nickname + " " + channelName + " :" + topic + "\r\n");
}

inline	std::string	rplEndNameList(std::string nickname, std::string channelName){
	return (":localhost 366 " + nickname + " " + channelName + " :End of /NAMES list\r\n");
}

inline	std::string errNoSuchChannel(std::string channelName){
	return (":localhost 403 " + channelName + " :Channel must be created without key. Set +k via MODE after joining\r\n");
}

inline	std::string errToManyChannels(std::string nickname, std::string channelName){
	return (":localhost 405 " + nickname + " " + channelName + " :You have joined too many channels\r\n");
}

inline	std::string errNoOrigin(std::string nickname){
	return (":localhost 409 " + nickname + " :No origin specified\r\n");
}

inline	std::string errNoRecipient(std::string nickname){
	return (":localhost 411 " + nickname + " :No recipient given\r\n");
}

inline	std::string errNoTextToSend(std::string nickname){
	return (":localhost 412 " + nickname + " :No text to send\r\n");
}

inline	std::string errUnknownCommand(std::string client, std::string command){
	return (":localhost 421 " + client + command + " :Unknown command\r\n");
}

inline	std::string	errNoNickNameGiven(void){
	return (":localhost 431 :No nickname given\r\n"); //might need to be changed
} 

inline	std::string	errOneUseNickname(void){
	return (":localhost 432 :Erroneous nickname\r\n");
}
 
inline	std::string	errNickNameInUse(std::string nickname, std::string nick){
	return (":localhost 433 " + nickname + " " + nick + " :Nickname is already in use\r\n");
}

inline	std::string	errNotRegistered(void){
	return (":localhost 451 :You have not registered\r\n");
}

inline	std::string	errNeedMoreParams(std::string command){
	return (":localhost 461 " + command + " :Not enough parameters\r\n");
}

inline	std::string errAlreadyRegistered(std::string nickname){
	return (":localhost 462 " + nickname + " :You may not reregister\r\n");
}

inline	std::string errPassMismatch(void){
	return ("localhost 464 :Password incorrect\r\n");
}

inline	std::string errChannelIsFull(std::string channelName){
	return (":localhost 471 " + channelName + " :Cannot join channel (+l)\r\n");
}

inline	std::string errInviteOnlyChan(std::string channelName){
	return (":localhost 471 " + channelName + " :Cannot join channel (+l)\r\n");
}

inline	std::string errBadChannelKey(std::string channelName){
	return (":localhost 475 " + channelName + " :Cannot join channel (+k)\r\n");
}

inline	std::string errBadChannelMask(std::string channelName){
	return (":localhost 476 " + channelName + " :Bad Channel Mask\r\n");
}

#endif
