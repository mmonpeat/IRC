#include "Channel.hpp"

//set modes to default
Channel::Channel(std::string name, Client* client) : _name(name), _topic("No topic is set"){
    addOperator(client);
	_limit_set = false;
	_invite_set = false;
	_topic_set = true;
	_password_set = false;
	_topic_init = false;
	_channel_creation_time = time(NULL);
    std::cout << "Channel " << this->_name << " constructor without password has been called" << std::endl;
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

//change the invite function
bool	Channel::isClientInvited(Client* client) const {
	for (std::vector<Client*>::const_iterator it = _invited_clients.begin(); it != _invited_clients.end(); ++it) {
		if (*it == client)
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

int 		Channel::getChannelLimit(void) const {
    return this->_channel_limit;
}

std::string	Channel::getTopic(void) const {
	return this->_topic;
}

bool		Channel::getTopicInit(void) const {
	if (_topic_init == true)
		return true;
	return false;
}

std::string	Channel::getChannelCreationTime(void) {
	std::ostringstream oss;
	oss << _channel_creation_time;
	std::string time = oss.str(); 
	return time;
}

std::string	Channel::getTopicSetter(void) {
	return _who_set_topic;
}

std::string	Channel::getTopicSetTime(void) {
	std::ostringstream	oss;
	oss << _topic_creation_time;
	std::string	time = oss.str();
	return time;
}

//---------------------------------- Setters -------------------------------------------------

void		Channel::setPasswordM(Client* op, const std::string& password) {
	this->_password = password;
	this->_password_set = true;
	passwordSetBroadcast(op);
	return;
}

void	Channel::setTopicMode(const std::string& op_nick) {
	this->_topic_set = true;

	std::string	message = ":" + op_nick + " MODE " + _name + "+t\r\n";
	broadcastMessage(message);
	return;
}

void	Channel::setLimitMode(int limit, const std::string& limit_str, const std::string& op_nick) {
	this->_channel_limit = limit;
    this->_limit_set = true;
	
	std::string message = ":" + op_nick + " MODE " + _name + " +l " + limit_str + "\r\n";
	broadcastMessage(message);
	return;
}

void	Channel::setInviteMode(const std::string& op_nick) {
	this->_invite_set = true;

	std::string	message = ":" + op_nick + " MODE " + _name + " +i\r\n";
	broadcastMessage(message);
	return;
}




//---------------------------------- Unsetters -----------------------------------------------

void	Channel::unsetPasswordMode(const std::string& op_nick) {
	this->_password.clear();
	this->_password_set = false;

	std::string	message = ":" + op_nick + " MODE " + _name + " -k\r\n";
	broadcastMessage(message);
	return;
}

void	Channel::unsetTopicMode(const std::string& op_nick) {
	this->_topic_set = false;

	std::string	message = ":" + op_nick + " MODE " + _name + " -t\r\n";
	broadcastMessage(message);
	return;
}

void	Channel::unsetLimitMode(const std::string& op_nick) {
	this->_channel_limit = 0;
	this->_limit_set = false;

	std::string	message = ":" + op_nick + " MODE " + _name + " -l\r\n";
	broadcastMessage(message);
	return;
}

void	Channel::unsetInviteMode(const std::string& op_nick) {
	this->_invite_set = false;
	_invited_clients.clear();

	std::string	message = ":" + op_nick + " MODE " + _name + " -i\r\n";
	broadcastMessage(message);
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

bool Channel::isClientInChannel(Client* client) const
{
    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getFd() == client->getFd())
            return true;
    }
    return false;
}

void Channel::removeClient(Client* client)
{
    // Eliminar de clientes normales
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getFd() == client->getFd()) {
            _clients.erase(it);
            break;
        }
    }
    
    // Eliminar de operadores si lo era
    for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if ((*it)->getFd() == client->getFd()) {
            _operators.erase(it);
            break;
        }
    }
}

void	Channel::removeOperator(Client *op) {
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if ((*it)->getNick() == op->getNick())
		{
			_operators.erase(it);
			break;
		}
	}
	return;
}

void	Channel::removeOperatorByNick(std::string& ex_op) {
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); it++) {
		if (equalNicks((*it)->getNick(), ex_op) == true) {
			removeOperator(*it);
			return;
		}
	}
	return;
}

void	Channel::removeInvited(Client* client) {
	for (std::vector<Client*>::iterator it = _invited_clients.begin(); it != _invited_clients.end(); it++) {
		if (*it == client) {
			_invited_clients.erase(it);
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

void	Channel::addOperatorByNick(const std::string& new_op) {
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (equalNicks((*it)->getNick(), new_op) == true) {
			addOperator(*it);
			return;
		}
	}
	return;
}


bool		Channel::isClient(Client *client) {
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNick() == client->getNick())
			return true;
	}
	return false;
}

bool		Channel::isClientByNick(std::string nick){
	if (nick.empty())
		return false;
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNick() == nick)
			return true;
	}
	return false;
}

bool		Channel::isChannelEmpty(void) const {
	bool	isEmpty = true;
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		isEmpty = false;
	}
	return (isEmpty);

}

void 		Channel::broadcastMessage(std::string message) const {
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it ) {
		if (*it)
			send((*it)->getFd(), message.c_str(), message.size(), 0);
	 }
	return;
}
void	Channel::msgtoChannel(std::string msg, int sender) const {
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it ) {
		if (*it && (*it)->getFd() != sender)
			send((*it)->getFd(), msg.c_str(), msg.size(), 0);
	 }
}

void		Channel::passwordSetBroadcast(Client* client) {
std::string	client_message =  ":" + client->getNick() + " MODE " + _name + " +k\r\n";
std::string	op_message = ":" + client->getNick() + " MODE " + _name + " +k "  + _password + "\r\n";

for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it ) {
		if (*it && (*it)->getNick() == client->getNick())
			send((*it)->getFd(), op_message.c_str(), op_message.size(), 0);
		else
			send((*it)->getFd(), client_message.c_str(), client_message.size(), 0);
	 }
	return;
}

void		Channel::displayTopic(void) const {
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it ) {
		send((*it)->getFd(), _topic.c_str(), _topic.size(), 0);
	}
	return;
}

int 		Channel::numberOfClients() const {
	return static_cast<int>(_clients.size());
}

std::string	Channel::returnModes(std::string nick) {
	std::string	modes;
	std::string	args;
	
	if (isInviteModeSet())
		modes += "i";
	if (isTopicModeSet())
		modes += "t";
	if (isPasswordSet()) {
		modes += "k";
		args += " " + _password;
	}
	if (isLimitModeSet()) {
		modes += "l";
		std::ostringstream oss;
		oss << _channel_limit;
		args += " " + oss.str();
	}
	if (isClientByNick(nick) == false)
		return modes.empty() ? "" : "+" + modes;
	return "+" + modes + args;
}


//---------------------------------- OPs Functions -------------------------------------------


bool Channel::isOperator(std::string nick) const {
	for (std::vector<Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (*it && equalNicks((*it)->getNick(), nick))
			return true;
	}
	return false;
}


void Channel::changeTopic(const std::string new_topic, Client* client) {
	_topic = new_topic;
	_topic_init = true;
	_topic_creation_time = time(NULL);
	std::string message = ":" + client->getNick() + " TOPIC " + _name + " :" + _topic + "\r\n";
	broadcastMessage(message);

	return;
}


void	Channel::kickUser(const std::string& kicker, const std::string& target) {
	std::string message = "KICK " + kicker + " " + _name + " " + target + "\r\n";
	broadcastMessage(message);

	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (equalNicks((*it)->getNick(), target)) {
			removeClient(*it);
			break;
		}	
	}
	return;
}

void		Channel::kickUserMsg(const std::string& kicker, const std::string& target, const std::string& comment) {
	std::string message = "KICK " + kicker + " " +  _name + " " + target + " :" + comment + "\r\n";
	broadcastMessage(message);
	
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (equalNicks((*it)->getNick(), target)) {
			removeClient(*it);
			break;
		}	
	}
	return;
}

void		Channel::inviteUser(Client *invited_client) {
	_invited_clients.push_back(invited_client);
	return;
}