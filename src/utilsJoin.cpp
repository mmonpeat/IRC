#include "Server.hpp"

void	Server::prepareForJoin(std::string *params, Client *client)
{
	std::vector<std::string> requestedChannels;
	std::vector<std::string> passChannels;
	
	if (params[1].empty())
	{
		sendReply(client->getFd(), errNeedMoreParams("JOIN"));
	}
	// if (!params[3].empty())
	// {
	// 	std::string err = "JOIN :Too many parameters. Use JOIN <channel> [key]\r\n";
	// 	sendReply(client->getFd(), err);
	// }
	if (!params[1].empty())
		requestedChannels = convertToVector(params[1]);

	// Comprova si hi ha password (no accedeixis fora límits)
	if (!params[2].empty())
		passChannels = convertToVector(params[2]);

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