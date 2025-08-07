#include "Server.hpp"

void    Server::invite(std::vector<std::string> params, Client *client){
    int         paramCount = params.size();
    Channel*    channel;

    if (paramCount < 3) {
        sendReply(client->getFd(), errNotEnoughParams(client->getNick(), "INVITE"));
        return;
    }

    channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}

    if (channel->isOperator(client->getNick()) == false) {
		sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
		return;
	}

    Client* invited = findClientByNick(params[2]);
    if (invited == NULL) {
        sendReply(client->getFd(), params[2] + " is not on this server\r\n");
        return;
    }

    // enviar mensaje al invitado
    //añadir a la lista de los invitados

    return;
}

Client* Server::findClientByNick(const std::string& nick) {
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (equalNicks(it->second->getNick(),nick))
            return it->second;
    }
    return NULL;
}