#include "Server.hpp"


bool Server::checkChannelNameRules(Client& client, const std::string& channelName)
{
	// Comprovar longitud
    if (channelName.empty() || channelName.length() > 50) 
	{
		std::string err = "476 ERR_BADCHANMASK " + channelName + " :Bad Channel Mask\r\n";
		sendReply(client.getFd(), err);
		return (false);
    }
	// Comprovar comenci amb #
	if (channelName[0] != '#')
	{
		std::string err = "476 ERR_BADCHANMASK " + channelName + " :Bad Channel Mask\r\n";
		sendReply(client.getFd(), err);
		return (false);
	}
	//caracters prohibits
	for (size_t j = 0; j < channelName.size(); ++j)
	{
		char c = channelName[j];
		if (c == ' ' || c == ',' || c == 7 || c == '\0' || c == '\r' || c == '\n' || c == ':')
		{
			std::string err = "476 ERR_BADCHANMASK " + channelName + " :Bad Channel Mask\r\n";
			sendReply(client.getFd(), err);
			return (false);
		}
	}
	return (true);
}

int Server::countClientChannels(Client& client, const std::vector<Channel>& channelsExistents)
{
	int count = 0;
    const std::string& clientNick = client.getNick();
    
    for (std::vector<Channel>::const_iterator it = channelsExistents.begin(); 
         it != channelsExistents.end(); ++it) 
    {
        const std::vector<std::string>& nickList = it->getClientNicks();
        if (std::find(nickList.begin(), nickList.end(), clientNick) != nickList.end()) {
            count++;
        }
    }
    return count;
}

void Server::checkModeToAddClient(Client& client, std::vector<Channel>& channelsExistents, std::string& channelName, std::string& channelPass)
{
	for(std::vector<Channel>::iterator it = channelsExistents.begin(); it != channelsExistents.end(); ++it)
	{
		if(it->getChannelName() == channelName)
		{
			bool canJoin = true;

			// Mode +i (invite-only)
			if (it->isInviteModeSet())
			{
				std::string err = "473 ERR_INVITEONLYCHAN " + channelName + " :Cannot join channel (+i)\r\n";
				sendReply(client.getFd(), err);
				canJoin = false;
			}

			// Mode +k (password)
			if (it->isPasswordSet())
			{
				if (channelPass.empty())
				{
					std::string err = "475 ERR_BADCHANNELKEY " + channelName + " :Cannot join channel (+k)\r\n";
					sendReply(client.getFd(), err);
					// std::cerr << "475 ERR_BADCHANNELKEY " << channelName << " :Cannot join channel (+k)" << std::endl;
					//ns on haauria d'anar la dde que el passa ja esta setado?
					//std::cerr << "467 ERR_KEYSET " << channelName << " :Channel key already set" << std::endl;
					canJoin = false;
				}
				else if (!it->isPasswordValidChannel(channelPass))
				{
					std::string err = "475 ERR_BADCHANNELKEY " + channelName + " :Cannot join channel (+k)\r\n";
					sendReply(client.getFd(), err);
					canJoin = false;
				}
			}

			// Mode +l (limit)
			//it->setChannelLimit(3);
			//si hi ha lloc i no esta ple +l (Ple: error: 471)
			//std::cout << "LIMIT: getChannelLimit: " << it->getChannelLimit() << " numberOfClients: " << it->numberOfClients(channelsExistents, channelName) << std::endl;
			if (it->isLimitModeSet())
			{
				int limit = it->getChannelLimit();
				int current = it->numberOfClients(channelsExistents, channelName);
				if (current >= limit)
				{
					std::string err = "471 ERR_CHANNELISFULL " + channelName + " :Cannot join channel (+l)\r\n";
					sendReply(client.getFd(), err);
					canJoin = false;
				}
			}

			// Afegir client si pot unir-se
			if (canJoin)
			{
				it->addClient(&client);
				//std::cerr << "affegeix client" << std::endl;

				// 1. JOIN a tots els del canal
				it->broadcastMessage(":" + client.getNick() + "!" + client.getUserName() + "@localhost JOIN " + channelName + "\r\n");

				// 2. TOPIC només al client
				if (!it->isTopicModeSet())
					sendReply(client.getFd(), "332 " + client.getNick() + " " + channelName + " :" + it->getTopic() + "\r\n");
				else
					sendReply(client.getFd(), "331 " + client.getNick() + " " + channelName + " :No topic is set\r\n");

				// 3. Llista d'usuaris (NAMREPLY + ENDOFNAMES)
				std::string names = "";
				const std::vector<std::string>& nickList = it->getClientNicks();
				for (size_t i = 0; i < nickList.size(); ++i)
				{
					if (i > 0) names += " ";
					names += nickList[i];
				}
				sendReply(client.getFd(), "353 " + client.getNick() + " = " + channelName + " :" + names + "\r\n");
				sendReply(client.getFd(), "366 " + client.getNick() + " " + channelName + " :End of /NAMES list\r\n");
			}
		}
	}
}

void Server::createNewChannel(Client& client, std::vector<Channel>& channelsExistents, const std::string& channelName, const std::string& channelPass)
{
	Channel newChannel(channelName, &client);
	newChannel.addOperator(&client); // primer usuari = operador
	newChannel.addClient(&client);

	if (!channelPass.empty())
	{
		newChannel.setPassword(channelPass);
		newChannel.setPasswordMode(true); // activa mode +k
	}
	channelsExistents.push_back(newChannel);
}

int Server::join(Client& client, std::vector<Channel> &channelsExistents, std::vector<std::string> ChannelsNames, std::vector<std::string> ChannelsPasswords)
{
	const int MAX_CHANNELS_PER_CLIENT = 5;//10

	int currentCount = countClientChannels(client, channelsExistents);
	//std::cout << "\nValors de en quants canals esta el client "<< client.getNick() << ":" << currentCount << "\n";
	int slotsLeft = MAX_CHANNELS_PER_CLIENT - currentCount;
	//std::cout << "\nNumero de canals als ques es pot afexir/crear:" << currentCount << "\n";

	if (slotsLeft <= 0) {
		std::string err = "405 ERR_TOOMANYCHANNELS " + client.getNick() + " :You have joined too many channels\r\n";
		sendReply(client.getFd(), err);
        return -1;
    }
	int validChannelsProcessed = 0;
	for (size_t i = 0; i < ChannelsNames.size() && slotsLeft > 0; i++)
	{
		std::string& channelName = ChannelsNames[i];
        if (!checkChannelNameRules(client, channelName)) {
            continue; // Saltar canals amb noms invàlids
        }
		// Si arribem aquí, el channel és vàlid
        validChannelsProcessed++;
		if (validChannelsProcessed > slotsLeft) {
			std::string err = "405 ERR_TOOMANYCHANNELS " + client.getNick() + " " + channelName + " :You have joined too many channels\r\n";
			sendReply(client.getFd(), err);
            continue;
		}
		
		std::string channelPass;
		if (i < ChannelsPasswords.size())
			channelPass = ChannelsPasswords[i];
		if (isChannelNameUnique(channelName, channelsExistents) == true && channelPass.empty()) {
			//std::cout << "\nNO existeix el channel, crearem un nou: "<< channelName << " Password: " << channelPass << " :hauria de ser null " << std::endl;
			createNewChannel(client, channelsExistents, channelName, channelPass);
		} else if (isChannelNameUnique(channelName, channelsExistents) != true) {
			channelName = getUniqueChannelName(channelName, channelsExistents);
			//std::cout << "\nJa existeix el channel: " << channelName << " Password: " << channelPass << std::endl;
			checkModeToAddClient(client, channelsExistents, channelName, channelPass);
		} else {
			// No permetre crear canals amb password
			std::string err = "403 ERR_NOSUCHCHANNEL " + channelName + " :Channel must be created without key. Set +k via MODE after joining\r\n";
			sendReply(client.getFd(), err);
		}
	}
	ChannelsNames.clear();
	ChannelsPasswords.clear();
    return (0);
}
