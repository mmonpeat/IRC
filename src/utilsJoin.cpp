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

bool	Server::isChannelNameUnique(std::string channelToCheck) const {
    for (std::map<int, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel* channel = it->second;
        if (channel && channel->getAuth()) {
			if (equalChannels(channel->getName(), channelToCheck))
				return false;
		}
	}
    return true;
}