#include "Server.hpp"

// control de topic -512

void    Server::topic(std::vector<std::string> params, Client *client){
    int 		paramCount = params.size();
	Channel*	channel;

	if (paramCount == 1) {
		sendReply(client->getFd(), errNotEnoughParams(client->getNick(), "TOPIC"));
		return;
	}

	channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}
    if (paramCount == 2) {
        showTopic(channel, client);
        return;
    }

    if (params[2].length() > 350) {
        sendReply(client->getFd(), client->getNick() + " :Topic too long. Max length is 350 characters\r\n");
        std::cout << params[2] << std::endl;
        std::cout << "!!!! len es" << params[2].length() << std::endl;
        return;
    }
   
    setTopic(channel, client, params[2]);
    return;
}

void    Server::showTopic(Channel* channel, Client* client) {
    if (channel->getTopicInit() == false)
        sendReply(client->getFd(), RPL_NOTOPIC(client->getNick(), channel->getChannelName()));
    else if (channel->getTopicInit()) {
        sendReply(client->getFd(), RPL_TOPIC(client->getNick(), channel->getChannelName(), channel->getTopic()));
        sendReply(client->getFd(), RPL_TOPICWHOTIME(client->getNick(), channel->getChannelName(), channel->getTopicSetter(), channel->getTopicSetTime()));
    }
    return;
}

void    Server::setTopic(Channel* channel, Client* client, const std::string& topic) {
    if (channel->isTopicModeSet()) {
        if(channel->isOperator(client->getNick()))
            channel->changeTopic(topic, client);
        else
            sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
    }
    else if (channel->isTopicModeSet() == false) {
        if(channel->isClientByNick(client->getNick()))
            channel->changeTopic(topic, client);
        else
            sendReply(client->getFd(), errNotOnChannel(client->getNick(), channel->getChannelName()));
    }
    return;
}
