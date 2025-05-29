#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Client.hpp"
#include <sys/socket.h>

// MAX_CHANNELS_PER_CLIENT 10

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

		std::string getName() const;
		std::string getRealName() const;

		void	addClient(Client *client);
		
		std::vector<std::string> 	getClientNicks() const;
};


#endif
