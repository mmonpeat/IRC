#include "Client.hpp"

//------------------------------ Constructor / Destructors------------------------

Client::Client(int fd): _fd(fd) {
	std::cout << "Client constructor called" << std::endl;
	_pass = false;
	_auth = false; // hasta que el cliente tenga todos los campos registrados
	return;
	}

Client::~Client ( void ) {
	std::cout << "Client destructor called" << std::endl; // mute later
	// delete all stored channels and etc..
	return;
}


//------------------------------ Setters -----------------------------------------

void		Client::setNick( std::string nick ) {
	this->_nick = nick;
	return;
}

void 		Client::setUserName(std::string user_name) {
	this->_username = user_name;
	return;
}

void		Client::setRealName( std::string real_name ) {
	this->_real_name = real_name;
	return;
}

void		Client::setAuth(bool status){
	this->_auth = status;
}

void		Client::setPass(bool status){
	this->_pass = status;
}

//------------------------------ Getters -----------------------------------------

std::string	Client::getNick(void) const {
	return (this->_nick);
}

std::string	Client::getUserName(void) const {
	return (this->_username);
}

std::string	Client::getRealName(void) const {
	return (this->_real_name);
}

int			Client::getFd(void) const {
	return (this->_fd);
}

bool		Client::getPass(void) const {
	return (this->_pass);
}

bool		Client::getAuth(void) const {
	return (this->_auth);
}


//------------------------------ Functions -----------------------------------------

void	Client::addToBuffer(char *buffer)
{
		_inputBuffer.append(buffer);
}
