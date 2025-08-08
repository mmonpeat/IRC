#include "Server.hpp"


Channel*	Server::getChannelByName(std::string& name) {
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
		if (equalNicks((*it)->getChannelName(), name) == true) // check it it works
			return (*it);
	}
	return NULL;
}

void	Server::channelModes(std::vector<std::string> params, Client *client) {
	std::cout << "!!!!!!!!!!!! HE ENTRADO EN MODE !!!!!!!!!" << std::endl; //delete later
	int 		paramCount = params.size();
	Channel*	channel;

	if (paramCount == 1) {
		sendReply(client->getFd(), errNotEnoughParams(client->getNick(), "MODE"));
		return;
	}

	channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}

	if (paramCount == 2) {
		sendReply(client->getFd(), RPL_CHANNELMODEIS(client->getNick(), channel->getChannelName(), channel->returnModes(client->getNick())));
		sendReply(client->getFd(), RPL_CREATIONTIME(client->getNick(), channel->getChannelName(), channel->getChannelCreationTime()));
		return;
	}
	if (channel->isOperator(client->getNick()) == false) {
		sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
		return;
	}
	applyModes(params, client, channel);
	return;
}


void	Server::applyModes(std::vector<std::string> params, Client *client, Channel* channel)
{
	std::cout << "Modes parameters are: " << params[2] << std::endl; //delete later
	
	const size_t	max_param_num = 3;
	size_t			found_param = 0;
	char			currentSign = 0;
	std::string 	validModes = "itkol";
	int				param_len = params.size();
	int				arg_i = 3;

	for (size_t i = 0; i < params[2].size(); ++i)
	{
		char c = params[2][i];

		if (c == '+' || c == '-')
			currentSign = c;
		else if (validModes.find(c) == std::string::npos || currentSign == 0)
			sendReply(client->getFd(), errNotKnownMode(client->getNick(), c));
		else if (((c == 'k' || c == 'l') && currentSign == '+') || c == 'o')
		{
			found_param++;
			if (found_param > max_param_num){
				std::string message = ":localhost " + client->getNick() + " MODE :Too many parameter-requiring modes in one command\r\n";
				sendReply(client->getFd(), message);
				return;
			}
			else if (arg_i >= param_len) {
				sendReply(client->getFd(),errNotEnoughParams(client->getNick(), "MODE")); // lack of parameter, maybe the message is different?
				return;
			}
			else 
				execMode(currentSign, c, params[arg_i++], client, channel);
		}
		else 
			execMode(currentSign, c, "empty", client, channel);
	}
	return;
}

void	Server::execMode(char sign, char c, std::string param, Client* client, Channel *channel){
	if (c == 'k')
		modeK(channel, param, sign, client);
	else if (c == 'o')
		modeO(channel, param, sign, client);
	else if (c == 'l')
		modeL(channel, param, sign, client);
	else if (c == 't')
		modeT(channel, sign, client);
	else if (c == 'i')
		modeI(channel, sign, client);
	return;
}

void	Server::modeK(Channel *channel, std::string password, char sign, Client *client) {
	// write another function for password checking
	if (password.empty() || password.size() > 32) { // not aked by the protocol but prevents problems
		std::string	message = ":" + client->getNick() + " :Password must be 1-32 characters long\r\n";
		sendReply(client->getFd(), message);
		return;
	}
	if (sign == '+')
		channel->setPasswordM(client, password);
	else if (channel->isPasswordSet() == true && sign == '-')
		channel->unsetPasswordMode(client->getNick());
	return;
}

void	Server::modeT(Channel *channel, char sign, Client *client) {
	if (channel->isTopicModeSet() == false && sign == '+')
		channel->setTopicMode(client->getNick());
	else if (channel->isTopicModeSet() == true && sign == '-')
		channel->unsetTopicMode(client->getNick());
	return;
}

bool Server::isLimitValid(const std::string& limit) {
    if (limit.empty() || limit.length() > 5)
        return false;
    for (size_t i = 0; i < limit.length(); i++) {
        if (!std::isdigit(limit[i]))
            return false;
    }
    // no leading zeros
    if (limit.length() > 1 && limit[0] == '0')
        return false;
    return true;
}


int		Server::strToInt(std::string arg) {
	return atoi(arg.c_str());
}

void	Server::modeL(Channel *channel, std::string arg, char sign, Client *client) {
	if (sign == '+') {
		if (isLimitValid(arg)) {
			int limit = strToInt(arg);
			if (limit > 0)
				channel->setLimitMode(limit, arg, client->getNick());
			else {
				std::string	message = ":" + client->getNick() + " :Limit should be a positive number\r\n";
				sendReply(client->getFd(), message);
				return;
			}
		}
		else {
			std::string	message = ":" + client->getNick() + " :Limit is not acceptable\r\n";
			sendReply(client->getFd(), message);
			return;
		}
	}
	else if (sign == '-' && channel->isLimitModeSet() == true)
		channel->unsetLimitMode(client->getNick());
	return;
}

void	Server::modeO(Channel *channel, std::string arg, char sign, Client *client) {
	if (channel->isClientByNick(arg) == false) {
		sendReply(client->getFd(), errUserNotInChannel(client->getNick(), channel->getChannelName()));
		return;
	}
	else if (sign == '+') {
		if (channel->isOperator(arg) == true)
			return;
		channel->addOperatorByNick(arg);
		std::string	message = ":" + client->getNick() + " MODE " + channel->getChannelName() + " +o " + arg + "\r\n";
		channel->broadcastMessage(message);
	}
	else if (sign == '-') {
		if (channel->isOperator(arg) == false)
			return;
		std::string	message = ":" + client->getNick() + " MODE " + channel->getChannelName() + " -o " + arg + "\r\n";
		channel->removeOperatorByNick(arg);
		channel->broadcastMessage(message);
	}
	return;
}

void	Server::modeI(Channel *channel, char sign, Client *client) {
	if (sign == '+') // add checker is +i is already on optional
		channel->setInviteMode(client->getNick());
	else if (sign == '-') //add checker if -i is already on optional
		channel->unsetInviteMode(client->getNick());
	return;
}
