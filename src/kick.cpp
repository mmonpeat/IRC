#include "Server.hpp"

// KICK #42chat Bob :Spamming

//check len  if there is a message

//check that the comment is not too big

//Sender not in channel	442 ERR_NOTONCHANNEL


void    Server::kick(std::string *params, Client *client) {
    int 		len = ptrLen(params);
	Channel*	channel;

	if (len < 3) {
		sendReply(client->getFd(), errNotEnoughParams(client->getNick(), "KICK"));
		return;
	}
    //which message should pop up first in case of 2 args, channel does not exist or lack of parameters
    channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}

	//check the len of comment before and not execute, or execute and send error to the op
    if (len >= 3) {
    	if (channel->isOperator(client->getNick()) == false) {
			sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
			return;
		}
		if (channel->isClientByNick(params[2]) == false) {
			sendReply(client->getFd(), errUserNotInChannel(client->getNick(), channel->getChannelName()));
			return;
		}
		if (len == 3)
			channel->kickUser(params[2]);
    }


    return;
}