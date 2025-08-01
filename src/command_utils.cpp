#include "Server.hpp"

bool		Server::isNickValid(std::string nick)
{
	// must not have: ',' '*' '?' '!' '@' also should not '.'
	std::string	forbidden[6] = {",", "*" , "?", "!", "@", "."};
	for (int i = 0; i < 6; i++)
	{
		if (nick.find(forbidden[i]) != std::string::npos)
		{
			return(false);
		}
	}
	//must not start with: '$' or ':' or '#'
	if (nick[0] == '$' || nick[0] == ':' || nick[0] == '#')
		return (false);
	return (true);
}

int			Server::findClient(std::string nick)
{
	for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (nick.compare(it->second->getNick()) == 0)
			return (it->second->getFd());
	}
	return (-1);	
}

Channel*	Server::findChannel(std::string channel_name)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channel_name.compare(channels[i]->getChannelName()) == 0)
			return (channels[i]);
	}
	return (NULL);	
}
