/* MODES EXPLAINED - DELETE LATER
	[] - i – Invite-only channel, only users who have been invited can join. 
		 We need to store a list of invited users (e.g., nicknames or fds)
		 Default: off 
	[] - t – Topic settable only by channel operators Prevents non-operators from changing the topic.
		 Default: on (+t)
	[] - k – Channel key (password) Users must provide a key (password) to join.  We need to store a std::string _key.
		 +k <key> sets the key, -k removes it.
		 Default: off
	[] - o – Give/take operator privilege. Used to promote (+o) or demote (-o) a user to/from channel operator.
		 You’ll be managing _operators (probably std::vector<Client*>)
		 Note: This mode applies to a user, not globally.
	[] - l – Set/remove user limit. Sets how many users can be in the channel.
		You need an int _userLimit, active only when +l is set. +l <number> sets limit, -l removes it.
		Default: off (no limit)
*/

/* Numeric replies for channel
	403 - trying to change topic and not being an operator when mode is resctricted

*/

#include "Channel.hpp"

//set modes to default
Channel::Channel(std::string name, Client* client) : _name(name), _topic("No topic is set"){
    //addOperator(Client* client)
    client->getNick(); //esta aqui de chorrada para que no se queje el make
	_limit_set = false;
	_invite_set = false;
	_topic_set = true;
	_password_set = false;
    std::cout << "Channel " << this->_name << " constructor has been called" << std::endl;
    return;
}

Channel::~Channel(void) {
    // check if there are allocated memory and delete it before destruction!!!
    std::cout << "Channel " << this->_name << " destructor has been called" << std::endl;
    return;
}


//---------------------------------- Class Functions -----------------------------------------

void	Channel::removeClient(Client* client) {

}

void	Channel::addClient(Client *client) {

}

bool	Channel::isClient(Client *client) {
	return true;
}

bool	Channel::isChannelEmpty(void) const {

	return true;
}
//---------------------------------- OPs Functions -------------------------------------------

bool    Channel::isOperator(std::string nick) const {
	for (std::vector<Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (*it && (*it)->getNick() == nick)
			return true;
	}
	return false;
}


void Channel::changeTopic(const std::string new_topic, Client* client) {
	if (isOperator(client->getNick())) {
		_topic = new_topic;

		std::string message = ":" + client->getNick() + " TOPIC #" + _name + " :" + _topic + "\r\n";

		for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if (*it)
				send((*it)->getFd(), message.c_str(), message.size(), 0);
		}
	}
}

//operators can kick other operators
void	Channel::kickUser(Client* kicker, Client* target) {
	if (isOperator(kicker->getNick()) == false) {
		std::cout << "You have no rights to kick users in this chat" << std::endl;
		return;
	}
	removeClient(target);
	//maybe message to users of the channel
	return;
}
