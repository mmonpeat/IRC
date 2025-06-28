#include "Server.hpp"

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

bool	Server::isChannelNameUnique(std::string& channelToCheck, const std::vector<Channel>& channelsExistents) const {
	for (std::vector<Channel>::const_iterator it = channelsExistents.begin(); it != channelsExistents.end(); ++it)
    {
		std::cout << "\nFuncio isChannelNameUnique:" << channelToCheck << std::endl;
		if (equalChannels(it->getChannelName(), channelToCheck))
			return false;
	}
    return true;
}

std::string	Server::getUniqueChannelName(std::string& channelToCheck, const std::vector<Channel>& channelsExistents) const {
    std::cout << "\nFuncio getUniqueChannelName:" << channelToCheck << std::endl;
	for (std::vector<Channel>::const_iterator it = channelsExistents.begin(); it != channelsExistents.end(); ++it)
    {
		if (equalChannels(it->getChannelName(), channelToCheck))
			return (it->getChannelName());
	}
    return (channelToCheck);
}