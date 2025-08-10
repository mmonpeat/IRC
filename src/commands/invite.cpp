#include "Server.hpp"

void    Server::invite(std::vector<std::string> params, Client *client){
    int         paramCount = params.size();
    Channel*    channel;
    Client*     invited;

    if (paramCount < 3) {
        sendReply(client->getFd(), errNotEnoughParams(client->getNick(), "INVITE"));
        return;
    }

    channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}

    if (channel->isInviteModeSet() == true && channel->isOperator(client->getNick()) == false) {
		sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
		return;
	}

    if (channel->isClient(client) == false)
    {
        sendReply(client->getFd(), errNotOnChannel(client->getNick(), channel->getChannelName()));
        return;
    }

    invited = findClientByNick(params[2]);
    if (invited == NULL) {
        sendReply(client->getFd(), params[2] + " is not on this server\r\n");
        return;
    }
    if (channel->isClient(invited)) { // this person is already on the channel
        sendReply(client->getFd(), errUserOnChannel(client->getNick(), channel->getChannelName(), invited->getNick()));
        return;
    }
    if (channel->isClientInvited(invited)) { //this person was already invited before 
        sendReply(client->getFd(), ":localhost NOTICE " + client->getNick() + " :Bob is already invited to " + channel->getChannelName() + "\r\n");
        return;
    }
        
    channel->inviteUser(invited);
    sendReply(client->getFd(), RPL_INVITING(client->getNick(), channel->getChannelName(), invited->getNick()));
    sendReply(invited->getFd(), client->getPrefix() + " INVITE " + invited->getNick() + " :" + channel->getChannelName() + "\r\n");
    return;
}

Client* Server::findClientByNick(const std::string& nick) {
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (equalNicks(it->second->getNick(),nick))
            return it->second;
    }
    return NULL;
}