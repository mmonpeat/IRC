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
    addOperator(client);
	_limit_set = false;
	_invite_set = false;
	_topic_set = true;
	_password_set = false;
    std::cout << "Channel " << this->_name << " constructor without password has been called" << std::endl;
    return;
}

Channel::Channel(std::string name, Client *client, std::string password) :  _name(name), _topic("No topic is set"), _password(password) {
	 addOperator(client);
	_limit_set = false;
	_invite_set = false;
	_topic_set = true;
	_password_set = true;
    std::cout << "Channel " << this->_name << " constructor with password has been called" << std::endl;
    return;
}


Channel::~Channel(void) {
    // check if there are allocated memory and delete it before destruction!!!
    std::cout << "Channel " << this->_name << " destructor has been called" << std::endl;
    return;
}



//---------------------------------- Modes setting getters and checkers -----------------------------------

bool	Channel::isLimitModeSet(void) const {
	if (this->_limit_set == true)
		return true;
	return false;
}

bool	Channel::isInviteModeSet(void) const {
	if (this->_invite_set == true)
		return true;
	return false;
}

bool	Channel::isTopicModeSet(void) const {
	if (this->_topic_set == true)
		return true;
	return false;
}

bool	Channel::isPasswordSet(void) const {
	if (this->_password_set == true)
		return true;
	return false;
}

bool	Channel::isPasswordValidChannel(std::string password) const {
	if (!password.empty() && !this->_password.empty() && (password == this->_password))
		return true;
	return false;
}

bool	Channel::isClientInvited(Client* client) const {
	for (std::vector<std::string>::const_iterator it = _invited_clients.begin(); it != _invited_clients.end(); ++it) {
		if (*it == client->getNick())
			return true;
	}
	return false;
}


//---------------------------------- Vriables getters -----------------------------------

std::string	Channel::getChannelName(void) const{
	return this->_name;
}
std::vector<std::string> Channel::getClientNicks() const
{
	std::vector<std::string> nicks;

	for (size_t i = 0; i < _clients.size(); ++i) {
		nicks.push_back(_clients[i]->getNick());
	}
	return nicks;
}

int 	Channel::getChannelLimit(void) const {
    return this->_channel_limit;
}

std::string	Channel::getTopic(void) const {
	return this->_topic;
}

//---------------------------------- Setters -------------------------------------------------
void	Channel::setChannelLimit(int limit) {
	this->_channel_limit = limit;
    this->_limit_set = true;
}

void Channel::setPassword(const std::string& password)
{
	this->_password = password;
	this->_password_set = true;
	//send message to the group
}

void Channel::setPasswordMode(bool mode)
{
	this->_password_set = mode;
}


//---------------------------------- Unsetters -----------------------------------------------

void		Channel::unsetPassword(const std::string& password) {
	if (this->_password == password) {
		this->_password.clear();
		this->_password_set = false;
		//send message to the group
	}
	else 
		std::cout << "The password in wrong" << std::endl;
	return;
}

//---------------------------------- Class Functions -----------------------------------------
void	Channel::addClient(Client *client) {
	this->_clients.push_back(client);
	std::string	message = client->getNick() + " (" + client->getRealName() + ") has joined \r\n";
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		send((*it)->getFd(), message.c_str(), message.size(), 0);
	}
	return;
}

void	Channel::removeClient(Client* client) {
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNick() == client->getNick()) {
			if (isOperator(client->getNick()))
				removeOperator(client);
			_clients.erase(it);
			break;
		}
	}
	// check if it is empty and call break to not print the message
	std::string	message = "deleted \r\n";
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		send((*it)->getFd(), message.c_str(), message.size(), 0);
	}
}

void	Channel::removeOperator(Client *op) {
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNick() == op->getNick())
		{
			_operators.erase(it);
			break;
		}
	}
}

void	Channel::addOperator(Client *new_op) {
	this->_operators.push_back(new_op);
	std::string	message = new_op->getNick() + " (" + new_op->getRealName() + ") has become operator \r\n"; //falta decir en que grup
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		send((*it)->getFd(), message.c_str(), message.size(), 0);
	}
	return;
}


bool	Channel::isClient(Client *client) {
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNick() == client->getNick())
			return true;
	}
	return false;
}

bool	Channel::isChannelEmpty(void) const {
	bool	isEmpty = true;
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		isEmpty = false;
	}
	return (isEmpty);

}

void 	Channel::broadcastMessage(std::string message) const {
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it ) {
		if (*it)
			send((*it)->getFd(), message.c_str(), message.size(), 0);
	 }
	return;
}

void	Channel::displayTopic(void) const {
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it ) {
		send((*it)->getFd(), _topic.c_str(), _topic.size(), 0);
	}
	return;
}

int	Channel::numberOfClients(std::vector<Channel>& channelsExistents, std::string& channelName)
{
	for(std::vector<Channel>::iterator it = channelsExistents.begin(); it != channelsExistents.end(); ++it)
	{
		if(it->getChannelName() == channelName)
		{
			return (static_cast<int>(it->_clients.size()));
		}
	}
	return (-1);
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
	if (isOperator(client->getNick()) || isTopicModeSet() == false) {
		_topic = new_topic;
		std::string message = ":" + client->getNick() + " TOPIC #" + _name + " :" + _topic + "\r\n";
		broadcastMessage(message);
		return;
	}
	return;
}

//operators can kick other operators
void	Channel::kickUser(Client* kicker, Client* target) {
	if (isOperator(kicker->getNick()) == false) {
		std::string message = " have no rights to kick users from this channel \r\n";
		broadcastMessage(message);
		return;
	}
	removeClient(target);
	return;
}
