#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Channel
{
	private:
		std::string		name;
		std::string[]		client_nicks;
		std::vector<Client> 	operators;
		std::vector<Client> 	clients;//if a client becomes an operator, it's removed from clients and added to operators.
	public:

};


#endif
