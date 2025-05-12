#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Client.hpp"

class Client;

class Channel
{
	private:
		std::string					_name;
		std::string					_topic;
		std::vector<std::string>	_client_nicks;
		std::vector<Client*> 		_operators;
		std::vector<Client*> 		_clients;//if a client becomes an operator, it's removed from clients and added to operators.
	public:
		Channel(std::string name, Client *client); //just an idea, as only clients can create channels
		~Channel(void);

		//void	addClient(Client *client);
		//void	removeClient()

		//functions
		// 1. bool isChannelEmpty(void) const; cada vez que alguien se desconecte o este 
		//kickeado mirar cuanta gente queda, en caso que zero eliminar el channel

		//2. void changeTopic(std::string topic); tiene que cambiar el topic

		//3. 
};


#endif
