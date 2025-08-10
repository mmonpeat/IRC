#include "Client.hpp"

//------------------------------ Constructor / Destructors------------------------

Client::Client(int fd): _fd(fd) {
	//std::cout << "Client constructor called" << std::endl;
	_pass = false;
	_auth = false; // hasta que el cliente tenga todos los campos registrados
	_end_connection = false; //flag raised to end connection
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

void		Client::setEnd(bool status){
	this->_end_connection = status;
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

std::string	Client::getPrefix(void) const {
	std::string prefix = ":" + this->getNick() + "!" + this->getUserName() + "@" + "localhost";
	return prefix;
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

bool		Client::getEnd(void) const {
	return (this->_end_connection);
}
//------------------------------ Functions -----------------------------------------

void	Client::addToBuffer(char *buffer)
{
		_inputBuffer.append(buffer);
}

bool	Client::readBuffer(std::string *msg)
{
	std::string::size_type	pos = _inputBuffer.find("\r\n");
	if (pos == std::string::npos)
		return (false);
	else
	{
		*msg = _inputBuffer.substr(0, pos);
		//if (msg->length() > 510)
		//	std::cout << "msg too long" << std::endl; //what do? send reply but which??
		_inputBuffer.erase(0, pos + 2);
		return (true);
	}
}
