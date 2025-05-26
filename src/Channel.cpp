#include "Channel.hpp"
#include "iostream"

Channel::Channel() {}

Channel::~Channel() {}

const std::vector<std::string>& Channel::getClientNicks() const 
{
	return client_nicks;
}