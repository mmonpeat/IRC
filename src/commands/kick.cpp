#include "Server.hpp"


//check that the comment is not too big

//Sender not in channel	442 ERR_NOTONCHANNEL


void    Server::kick(std::vector<std::string> params, Client *client) {
    int 		paramCount = params.size();
	Channel*	channel;

	if (paramCount == 1) {
		sendReply(client->getFd(), errNotEnoughParams(client->getNick(), "KICK"));
		return;
	}
    //which message should pop up first in case of 2 args, channel does not exist or lack of parameters
    channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}
    if (paramCount >= 3) {
    	if (channel->isOperator(client->getNick()) == false) {
			sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
			return;
		}
		if (channel->isClientByNick(params[2]) == false) {
			sendReply(client->getFd(), errUserNotInChannel(client->getNick(), channel->getChannelName()));
			return;
		}
		if (paramCount == 3)
			channel->kickUser(client->getNick(), params[2]);
		if (paramCount == 4) {
			if (params[3].length() < 301)
				channel->kickUserMsg(client->getNick(), params[2], params[3]);
			else
				sendReply(client->getFd(), ":localhost " + client->getNick() + " :Message exceeds 300 characters\r\n");
		}
    }


    return;
}