#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include "Client.hpp"

class Client;

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::vector<Client*>	_operators;
		std::vector<Client*>	_clients;
		//modes
		bool					_limit_set;
		bool					_invite_set;
		bool					_topic_set;
		bool					_password_set;
		//vector of invited clients
		int						_channel_limit;
		std::string				_password;
		
	public:
		Channel(std::string name, Client *client);
		~Channel(void);

		void	addClient(Client *client);
		void	removeClient(Client* client);
		void	addOperator(Client *new_op);
		void	removeOperator(Client *op);
		bool	isClient(Client* client);
		bool	isChannelEmpty(void) const; //llamar cada vez que alguien haga quit
		

		// Channel functions for ops
		bool	isOperator(std::string nick) const;
		void	changeTopic(std::string topic, Client* client);
		void	kickUser(Client* kicker, Client* target);
		//void	changeMode(Client* client, std::string command);
};


#endif
