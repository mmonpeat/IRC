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
		std::vector<Client*> 		_clients;
	public:
		Channel(std::string name, Client *client); //just an idea, as only clients can create channels
		~Channel(void);

		//void	addClient(Client *client);
		//void	removeClient()

		//functions
		// 1. bool isChannelEmpty(void) const; llamar cada vez que alguien haga quit

	
		// Channel functions for ops
		bool	hasPerms();
		void	changeTopic(std::string topic, std::string nick);
		//void	kickUser();
		//void	changeMode();
};


#endif
