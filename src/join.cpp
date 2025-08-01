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

			// Mode +i (invite-only)
			if (channel->isInviteModeSet())
			{
				sendReply(client.getFd(), errInviteOnlyChan(channelName));
				canJoin = false;
			}

			// Mode +k (password)
			if (channel->isPasswordSet())
			{
				if (channelPass.empty())
				{
					sendReply(client.getFd(), errBadChannelKey(channelName));
					// std::cerr << "475 ERR_BADCHANNELKEY " << channelName << " :Cannot join channel (+k)" << std::endl;
					//ns on haauria d'anar la dde que el passa ja esta setado?
					//std::cerr << "467 ERR_KEYSET " << channelName << " :Channel key already set" << std::endl;
					canJoin = false;
				}
				else if (!channel->isPasswordValidChannel(channelPass))
				{
					sendReply(client.getFd(), errBadChannelKey(channelName));
					canJoin = false;
				}
			}

			// Mode +l (limit)
			//channel->setChannelLimit(3);
			//si hi ha lloc i no esta ple +l (Ple: error: 471)
			//std::cout << "LIMIT: getChannelLimit: " << channel->getChannelLimit() << " numberOfClients: " << channel->numberOfClients(channelsExistents, channelName) << std::endl;
			if (channel->isLimitModeSet())
			{
				int limit = channel->getChannelLimit();
				int current = channel->numberOfClients();
				if (current >= limit)
				{
					sendReply(client.getFd(), errChannelIsFull(channelName));
					canJoin = false;
				}
			}
			
			if (canJoin)
			{
				// Afegir client al canal
				channel->addClient(&client);

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
		}
	}
}

void Server::createNewChannel(Client& client, std::vector<Channel*>& channelsExistents, const std::string& channelName, const std::string& channelPass)
{
    Channel* newChannel = new Channel(channelName, &client);
    newChannel->addClient(&client);
    newChannel->addOperator(&client);
    
    if (!channelPass.empty()) {
        newChannel->setPassword(channelPass);
        newChannel->setPasswordMode(true);
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
		// if (isChannelNameUnique(channelName, channelsExistents) == true && channelPass.empty()) {
		// 	//std::cout << "\nNO existeix el channel, crearem un nou: "<< channelName << " Password: " << channelPass << " :hauria de ser null " << std::endl;
		// 	createNewChannel(client, channelsExistents, channelName, channelPass);
		// } else if (isChannelNameUnique(channelName, channelsExistents) != true) {
		// 	channelName = getUniqueChannelName(channelName, channelsExistents);
		// 	checkModeToAddClient(client, channelsExistents, channelName, channelPass);
		// } else {
		// 	// No permetre crear canals amb password
		// 	std::string err = "403 ERR_NOSUCHCHANNEL " + channelName + " :Channel must be created without key. Set +k via MODE after joining\r\n";
		// 	sendReply(client.getFd(), err);
		// }
	}
	ChannelsNames.clear();
	ChannelsPasswords.clear();
    return (0);
}
