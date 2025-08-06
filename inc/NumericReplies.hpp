#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP


//might not use the numbers but for now i'm leaving them here
//command codes
# define	RPL_WELCOME				001

//error codes
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

//Messages
inline	std::string	rplWelcome(std::string nickname){
	return (":localhost 001 " + nickname + " :Welcome to our IRC server, " + nickname + ":)\r\n");
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


// --------------------------------------- Mode Replies-------------------------------------------


inline std::string	errNotOperator(const std::string& clientNick, const std::string& channelName) {
	return (":localhost 482 "  + clientNick + " " + channelName + " :You're not channel operator\r\n");
}


inline std::string	errChannelNotExist(const std::string& clientNick, const std::string& channelName) {
	return (":localhost 403 " + clientNick + " " + channelName + " :No such channel\r\n");
}

inline std::string	errUserNotInChannel(const std::string& clientNick, const std::string& channelName) {
	return (":localhost 401 " + clientNick + " " + channelName + " :They aren't on that channel\r\n");
}

inline std::string	errNotEnoughParams(const std::string& clientNick) {
	return (":localhost 461 " + clientNick + " MODE :Not enough parameters\r\n");
}

inline std::string	RPL_CHANNELMODEIS(const std::string& clientNick, const std::string& channelName, const std::string modes) {
	return (":localhost 324 " + clientNick + " " + channelName + " " + modes + "\r\n");
}

inline std::string	RPL_CREATIONTIME(const std::string& clientNick, const std::string& channelName, const std::string& timeStamp) {
	return (":localhost 329 " + clientNick + " " + channelName + " " + timeStamp + "\r\n" );
}

inline std::string	errNotKnownMode(const std::string& clientNick, const char& modeChar) {
	return (":localhost 472 " + clientNick + " " + modeChar + " :is unknown mode char to me\r\n");
}

// --------------------------------------- Topic Replies-------------------------------------------

inline std::string	RPL_NOTOPIC(const std::string& clientNick, const std::string& channelName) {
	return (":localhost 331 " + clientNick + " " + channelName + " :No topic is set\r\n");
}

inline std::string	RPL_TOPIC(const std::string& clientNick, const std::string& channelName, const std::string& topic) {
	return (":localhost 332 " + clientNick + " " + channelName + " :" + topic + "\r\n");
}

inline std::string RPL_TOPICWHOTIME(const std::string& clientNick, const std::string& channelName, const std::string& topicSetter, std::string topicSetTime) {
	return (":localhost 333 " + clientNick + " " + channelName + " " + topicSetter + " " + topicSetTime + "\r\n");
}

inline std::string	errNotOnChannel(const std::string& clientNick, const std::string& channelName) {
	return (":localhost 442 " + clientNick + " " + channelName + " :You're not on that channel\r\n");
}

#endif
