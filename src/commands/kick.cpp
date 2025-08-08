#include "Server.hpp"


void    Server::kick(std::vector<std::string> params, Client *client) {
    int 		paramCount = params.size();
	Channel*	channel;

	if (paramCount == 1) {
		sendReply(client->getFd(), errNotEnoughParams(client->getNick(), "KICK"));
		return;
	}

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
		//std::cout << "!!!!!!!!!!!!!!!!!!!!  0" << params[0] << std::endl;
		//std::cout << "!!!!!!!!!!!!!!!!!!!!  1" << params[1] << std::endl;
		//std::cout << "!!!!!!!!!!!!!!!!!!!!  2" << params[2] << std::endl;
		//if (paramCount == 4)
			//std::cout << "!!!!!!!!!!!!!!!!!!!!  3" << params[3] << std::endl;

		if (channel->isClientByNick(params[2]) == false) {
			//std::cout << "!!!!!!!!!!!!!!!!!!!!" << params[2] << std::endl;
			sendReply(client->getFd(), errUserNotInChannel(client->getNick(), channel->getChannelName()));
			return;
		}
		if (paramCount == 3)
			channel->kickUser(client->getPrefix(), params[2]);
		if (paramCount == 4) {
			if (params[3].length() < 301)
				channel->kickUserMsg(client->getNick(), params[2], params[3]);
			else
				sendReply(client->getFd(), ":localhost " + client->getNick() + " :Message exceeds 300 characters\r\n");
		}
    }


    return;
}