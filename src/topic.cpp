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
            sendReply(client->getFd(), RPL_TOPICWHOTIME(client->getNick(), channel->getChannelName(), channel->getTopicSetter(), channel->getTopicSetTime()));
        }
        return;
    }
    //controlar el topic len
    if (len > 2) {
        if (channel->isTopicModeSet()) {
            if(channel->isOperator(client->getNick()))
                channel->changeTopic(params[2], client);
            else
                sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
            return;
        }
        else if (channel->isTopicModeSet() == false) {
            if(channel->isClientByNick(client->getNick()))
               channel->changeTopic(params[2], client);
            else
                sendReply(client->getFd(), errNotOnChannel(client->getNick(), channel->getChannelName()));
        }
    }
    return;
}


