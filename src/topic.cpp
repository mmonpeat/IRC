#include "Server.hpp"

void    Server::topic(std::string *params, Client *client){
    int 		len = ptrLen(params);
	Channel*	channel;

	if (len == 1) {
		sendReply(client->getFd(), errNotEnoughParams(client->getNick()));
		return;
	}
	channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}
    if (len == 2) {
        if (channel->getTopicInit() == false)
            sendReply(client->getFd(), RPL_NOTOPIC(client->getNick(), channel->getChannelName()));
        else if (channel->getTopicInit()) {
            sendReply(client->getFd(), RPL_TOPIC(client->getNick(), channel->getChannelName(), channel->getTopic()));
            //sendReply(client->getFd(), RPL_TOPICWHOTIME()); time and creator of the topic
        }
        return;
    }
    if (len > 2) {
        if (channel->isTopicModeSet()) {
            if(channel->isOperator(client->getNick()))
                // puede setear el topic, guargar tiempo y el nick, topic init = true
            else {
                //error_message not an operator
            }
            return;
        }
        else if (channel->isTopicModeSet() == false) {
            if(channel->isClientByNick(client->getNick()))
                 // puede setear el topic, guargar tiempo y el nick, topic init = true
            else
                //error_message not a client
        }
    }
    return;
}


