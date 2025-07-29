#include "Server.hpp"

int		Server::ptrLen(std::string *ptr) {
	int	len = 0;


	while (ptr[len] != "\0")
	{
		std::cout << ptr[len] << std::endl;
		len++;
	}
	return len;
}

Channel*	Server::getChannelByName(std::string& name) {
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
		if (equalNicks((*it)->getChannelName(), name) == true) // check it it works
			return (*it);
	}
	return NULL;
}


void	Server::channelModes(std::string *params, Client *client) {
	int 		len = ptrLen(params);
	Channel*	channel;

	if (len < 2) //handdle later
		return;
	channel = getChannelByName(params[1]);
	if (channel == NULL) {
		sendReply(client->getFd(), errChannelNotExist(client->getNick(), params[1]));
		return;
	}
	if (channel->isOperator(client->getNick()) == false) {
		sendReply(client->getFd(), errNotOperator(client->getNick(), channel->getChannelName()));
		return;
	}
	applyModes(params, client, channel); // maybe better void
	// if len == 2
	// handdle /MODE #channel that does exist (if you are an op, if you are a part of that channel and if you are not part of that channel)
	return;
}

//  /MODE #chanelName +ok-l Borja password 

void	Server::applyModes(std::string *params, Client *client, Channel* channel)
{
	std::cout << "Modes parameters are: " << params[2] << std::endl; //delete later
	
	const size_t	max_param_num = 3;
	size_t			found_param = 0;
	char			currentSign = 0;
	std::string 	validModes = "itkol";
	int				param_len = ptrLen(params);
	int				arg_i = 3;

	for (size_t i = 0; i < params[2].size(); ++i)
	{
		char c = params[2][i];

		if (c == '+' || c == '-')
			currentSign = c;
		else if (validModes.find(c) == std::string::npos || currentSign == 0)
			std::cout << "Error: invalid mode character '" << c << "'" << std::endl;
		else if (c == 'k' || (c == 'l' && currentSign == '+') || c == 'o')
		{
			found_param++;
			if (found_param > max_param_num)
				std::cout << "Error: too many parameter-requiring modes" << std::endl; //delete later
			else if (arg_i >= param_len) 
				std::cout << "Error: too many parameter-requiring modes" << std::endl; //delete later
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
	//else if (c = 'o')
		//modeO()
	else if (c == 'l')
		modeL(channel, param, sign, client);
	else if (c == 't')
		modeT(channel, sign, client);
	else if (c == 'i')
		std::cout << "Function to be written" << std::endl;
	return;

}


//461 ERR_NEEDMOREPARAMS if no parameter for k
void	Server::modeK(Channel *channel, std::string password, char sign, Client *client) {
	if (channel->isPasswordSet() == false && sign == '+') {
		channel->setPassword(password);
	}
	// check what happens in real servers if mode is on and another +k comes and same for -k
	else if (channel->isPasswordSet() == true && sign == '-') {
		channel->unsetPassword(password);
	}
	(void) client;
	return;
}

void	Server::modeT(Channel *channel, char sign, Client *client) {
	if (channel->isTopicModeSet() == false && sign == '+')
		channel->setTopicMode();
	else if (channel->isTopicModeSet() == true && sign == '-')
		channel->unsetTopic();
	return;
	(void) client; // check if needed
}

bool	Server::isLimitValid(std::string limit) {
	if (limit.empty() == true || limit.length() > 9)
		return false;
	size_t i = 0;
	if (limit[0] == '+')
		i = 1;
	for (; i < limit.length(); i++) {
		if (!std::isdigit(limit[i]))
			return false;
	}
	return true;
}

int		Server::strToInt(std::string arg) {
	if (arg == "0")
		return 0;
	return atoi(arg.c_str());
}

void	Server::modeL(Channel *channel, std::string arg, char sign, Client *client) {
	if (sign == '+' && isLimitValid(arg) == true)
		channel->setChannelLimit(strToInt(arg));
	else if (channel->isLimitModeSet() == true)
		channel->unsetLimit();
	(void) client;
	return;
}

/*void	Server::modeO(Channel *channel, std::string arg, char sign, Client *client) {
	if (channel->isClientByNick(arg) == false)
		return;
	channel->addOperator()
}*/


// LIMIT 3 modes that require parameter
/* input /MODE #pepe +k+l-t+i+o koko 8 poter Polly2 
 no msg for too many parameters but only executes 3 modes that requiere parameters
	output: for all users of the channel
* Polly sets mode +i on #pepe
* Polly sets mode -t on #pepe
* Polly sets channel keyword to koko
* Polly sets channel limit to 8 */

/* Faulty parameters 



*/

// MODE K -k requieres a password to remove the mode 
/*
	Polly2 sets channel keyword to JOhny
	Polly2 removes channel keyword
*/

/*
Code	Name	When it's used
324	RPL_CHANNELMODEIS	Sent in response to a MODE request, shows current modes (including +k)
467	ERR_KEYSET	Sent when trying to set +k but a key is already set (optional behavior)
441	ERR_USERNOTINCHANNEL	If trying to modify modes related to a user not in the channel
442	ERR_NOTONCHANNEL	If the client is not on the channel
443	ERR_USERONCHANNEL	If trying to give mode to a user already with it
461	ERR_NEEDMOREPARAMS	When not enough parameters are provided (e.g. /MODE #chan +k with no key)
472	ERR_UNKNOWNMODE	If the mode character is unknown (e.g. typo like /MODE #chan +x)
482	ERR_CHANOPRIVSNEEDED	If a non-operator tries to change the channel mode
*/