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


void	Channel::addClient(Client *client) {
	this->_clients.push_back(client);
	std::string	message = client->getNick() + " (" + client->getRealName() + ") has joined \r\n";
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		send((*it)->getFd(), message.c_str(), message.size(), 0);
	}
	return;
}


std::vector<std::string> Channel::getClientNicks() const
{
	std::vector<std::string> nicks;
	for (size_t i = 0; i < _clients.size(); ++i) {
		nicks.push_back(_clients[i]->getNick());
	}
	return nicks;
}


std::string Channel::getName() const { return _name; }

