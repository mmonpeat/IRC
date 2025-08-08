#include "Server.hpp"

void	Server::prepareForJoin(std::vector<std::string> params, Client *client)
{
	int len = params.size();
	std::vector<std::string> requestedChannels;
	std::vector<std::string> passChannels;

	if (len == 1)
		sendReply(client->getFd(), errNeedMoreParams("JOIN"));
	if (len >= 3)
	{
		requestedChannels = convertToVector(params[1]);
		passChannels = convertToVector(params[2]);
	} else if (len == 2)
		requestedChannels = convertToVector(params[1]);

	join(*client, this->channels, requestedChannels, passChannels);
}


std::vector<std::string> Server::convertToVector(const std::string& line)
{
	std::vector<std::string> result;
	std::stringstream ss(line);
	std::string channel;

	while (std::getline(ss, channel, ',')) {
		if (!channel.empty())
			result.push_back(channel);
	}
	return (result);
}

bool	Server::equalChannels(std::string new_channel, std::string channel) const {
	if (new_channel == channel)
		return true;
	if (new_channel.size() != channel.size())
		return false;
	for (size_t i = 0; i < channel.size(); i++) {
		if (foldChar(new_channel[i]) != foldChar(channel[i]))
			return false;
	}
	return true;
} 

bool	Server::isChannelNameUnique(std::string& channelToCheck, const std::vector<Channel*>& channelsExistents) const {
	for (std::vector<Channel*>::const_iterator it = channelsExistents.begin(); it != channelsExistents.end(); ++it)
    {
		if (equalChannels((*it)->getChannelName(), channelToCheck))
			return false;
	}
    return true;
}

std::string	Server::getUniqueChannelName(std::string& channelToCheck, const std::vector<Channel*>& channelsExistents) const {
	for (std::vector<Channel*>::const_iterator it = channelsExistents.begin(); it != channelsExistents.end(); ++it)
    {
		if (equalChannels((*it)->getChannelName(), channelToCheck))
			return ((*it)->getChannelName());
	}
    return (channelToCheck);
}

bool	Server::checkModeChannelPassword(Channel* channel, const std::string& pass, Client& client)
{
	(void)channel;
	(void)client;
	(void)pass;
	return(false);
}
bool	Server::checkModeChannelLimit(Channel* channel, Client& client)
{
	(void)channel;
	(void)client;
	return(false);
}
bool	Server::checkModeInvite(Channel* channel, Client& client)
{
	(void)channel;
	(void)client;
	return(false);
}
void	Server::replayMsgBecauseClientAddedToChannel(Channel* channel, Client& client, std::string channelName)
{
	// Mensaje JOIN para TODOS incluyendo al nuevo cliente
	std::string joinMsg = ":" + client.getNick() + "!" + client.getUserName() + "@localhost JOIN " + channelName + "\r\n";
	channel->broadcastMessage(joinMsg);
	
	// 1. JOIN a tots els del canal
	sendReply(client.getFd(), joinMsg);

	// 2.1 TOPIC Si es el primer usuario, hacerlo operador
	if (channel->getClientCount() == 1) {
		channel->addOperator(&client);
		sendReply(client.getFd(), "MODE " + channelName + " +o " + client.getNick() + "\r\n");
		
		// Notificar a todos que es operador
		channel->broadcastMessage(":" + client.getNick() + "!" + client.getUserName() + "@localhost MODE " + channelName + " +o " + client.getNick() + "\r\n");
	}

	// Enviar información del canal (TOPIC)
	if (!channel->getTopic().empty()) {
		sendReply(client.getFd(), rplTopicSet(client.getNick(), channelName, channel->getTopic()));
	} else {
		sendReply(client.getFd(), rplNoTopicSet(client.getNick(), channelName));
	}

	// 3. Llista d'usuaris (NAMREPLY)
	std::string namesMsg = ":localhost 353 " + client.getNick() + " = " + channelName + " :";
	const std::vector<std::string>& nickList = channel->getClientNicks();
	for (size_t i = 0; i < nickList.size(); ++i) {
		if (i > 0) namesMsg += " ";
		if (channel->isOperator(nickList[i])) {
			namesMsg += "@";
		}
		namesMsg += nickList[i];
	}
	sendReply(client.getFd(), rplNamesMsg(namesMsg));
	sendReply(client.getFd(), rplEndNameList(client.getNick(), channelName));
}
