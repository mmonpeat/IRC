#include "Server.hpp"


bool Server::checkChannelNameRules(Client& client, const std::string& channelName)
{
	// Comprovar longitud
    if (channelName.empty() || channelName.length() > 50) 
	{
		sendReply(client.getFd(), errBadChannelMask(channelName));
		return (false);
    }
	// Comprovar comenci amb #
	if (channelName[0] != '#')
	{
		sendReply(client.getFd(), errBadChannelMask(channelName));
		return (false);
	}
	//caracters prohibits
	for (size_t j = 0; j < channelName.size(); ++j)
	{
		char c = channelName[j];
		if (c == ' ' || c == ',' || c == 7 || c == '\0' || c == '\r' || c == '\n' || c == ':')
		{
			sendReply(client.getFd(), errBadChannelMask(channelName));
			return (false);
		}
	}
	return (true);
}

int Server::countClientChannels(Client& client, const std::vector<Channel*>& channelsExistents)
{
	int count = 0;
    const std::string& clientNick = client.getNick();
    
    for (std::vector<Channel*>::const_iterator it = channelsExistents.begin(); 
         it != channelsExistents.end(); ++it) 
    {
        const std::vector<std::string>& nickList = (*it)->getClientNicks();
        if (std::find(nickList.begin(), nickList.end(), clientNick) != nickList.end()) {
            count++;
        }
    }
    return count;
}

void Server::checkModeToAddClient(Client& client, std::vector<Channel*>& channelsExistents, std::string& channelName, std::string& channelPass)
{
	for (std::vector<Channel*>::iterator it = channelsExistents.begin(); it != channelsExistents.end(); ++it) {
        Channel* channel = *it;
		if (equalChannels(channel->getChannelName(), channelName)) 
		{
			// Verificación crítica que evita segfaults
			if (channel->getChannelName() != channelName) {
				return;
			}
			bool canJoin = true;

			/*
				if (!checkModeChannelPassword(channel, channelPass, client)) break;
				if (!checkModeChannelLimit(channel, client)) break;
				if (!checkModeInvite(channel, client)) break;
			*/
			// Mode +k (password)
			if (channel->isPasswordSet())
			{
				if (channelPass.empty())
				{
					sendReply(client.getFd(), errBadChannelKey(channelName));
					canJoin = false;
				}
				else if (!channel->isPasswordValidChannel(channelPass))
				{
					sendReply(client.getFd(), errBadChannelKey(channelName));
					canJoin = false;
				}
			}

			// Mode +l (limit)
			std::cout << "cha lim:" <<  channel->getChannelLimit()<< "num client in cha" << channel->numberOfClients() << std::endl;
			if (channel->isLimitModeSet())
			{
				int limit = channel->getChannelLimit();
				int current = channel->numberOfClients();
				std::cout << "hola imprimeix limit" << std::endl;
  				if (current >= limit)
				{
					sendReply(client.getFd(), errChannelIsFull(channelName));
					canJoin = false;
				}
			}
			
			// Mode +i (invite-only)
			if (channel->isInviteModeSet() == true)
			{
				if (channel->isClientInvited(&client) == false) {
					sendReply(client.getFd(), errInviteOnlyChan(channelName));
					canJoin = false;
				} else if (channel->isClientInvited(&client) == true) {
					channel->removeInvited(&client);
				}
			} else if (channel->isClientInvited(&client) == true){
				channel->removeInvited(&client);
			}

			if (canJoin)
			{
				// Afegir client al canal
				channel->addClient(&client);
				replayMsgBecauseClientAddedToChannel(channel, client, channelName);
			}
		}
	}
}

void Server::createNewChannel(Client& client, std::vector<Channel*>& channelsExistents, const std::string& channelName, const std::string& channelPass)
{
    Channel* newChannel = new Channel(channelName, &client);
    newChannel->addClient(&client);
    newChannel->addOperator(&client);
    
    if (!channelPass.empty()) {
        sendReply(client.getFd(), errNoSuchChannel(channelName));
    }

    channelsExistents.push_back(newChannel);
	// Notificar al cliente que ha creado el canal
    std::string joinMsg = ":" + client.getNick() + "!" + client.getUserName() + "@localhost JOIN " + channelName + "\r\n";
    sendReply(client.getFd(), joinMsg);
    
    // Enviar información del canal al creador
    sendReply(client.getFd(), rplNoTopicSet(client.getNick(), channelName));
    
    std::string namesMsg = ":localhost 353 " + client.getNick() + " = " + channelName + " :@" + client.getNick();
    sendReply(client.getFd(), rplNamesMsg(namesMsg));
    sendReply(client.getFd(), rplEndNameList(client.getNick(), channelName));
    
    // Notificar modo operador
    sendReply(client.getFd(), "MODE " + channelName + " +o " + client.getNick() + "\r\n");
}

int Server::join(Client& client, std::vector<Channel*>& channelsExistents, std::vector<std::string> ChannelsNames, std::vector<std::string> ChannelsPasswords)
{
	const int MAX_CHANNELS_PER_CLIENT = 5;//10

	int currentCount = countClientChannels(client, channelsExistents);
	//std::cout << "\nValors de en quants canals esta el client "<< client.getNick() << ":" << currentCount << "\n";
	int slotsLeft = MAX_CHANNELS_PER_CLIENT - currentCount;
	//std::cout << "\nNumero de canals als ques es pot afexir/crear:" << currentCount << "\n";

	int validChannelsProcessed = 0;
	for (size_t i = 0; i < ChannelsNames.size() && slotsLeft > 0; i++)
	{
		std::string& channelName = ChannelsNames[i];
		if (slotsLeft <= 0) {
			sendReply(client.getFd(), errToManyChannels(client.getNick(), channelName));
			return (-1);
    	}
        if (!checkChannelNameRules(client, channelName)) {
            continue; // Saltar canals amb noms invàlids
        }
		// Si arribem aquí, el channel és vàlid
        validChannelsProcessed++;
		if (validChannelsProcessed > slotsLeft) {
			sendReply(client.getFd(), errToManyChannels(client.getNick(), channelName));
            continue;
		}
		
		std::string channelPass;
		if (i < ChannelsPasswords.size())
			channelPass = ChannelsPasswords[i];
		if (isChannelNameUnique(channelName, channelsExistents)) {
            if (channelPass.empty()) {
                createNewChannel(client, channelsExistents, channelName, channelPass);
            } else {
                sendReply(client.getFd(), errNoSuchChannel(channelName));
            }
        } else {
            std::string actualName = getUniqueChannelName(channelName, channelsExistents);
            checkModeToAddClient(client, channelsExistents, actualName, channelPass);
        }
	}
	ChannelsNames.clear();
	ChannelsPasswords.clear();
    return (0);
}
