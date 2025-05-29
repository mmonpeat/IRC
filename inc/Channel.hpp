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
		std::string					_name;
		std::string					_topic;
		std::vector<Client*>		_operators;
		std::vector<Client*>		_clients;
	
		// Mode control 
		bool						_limit_set;
		bool						_invite_set;
		bool						_topic_set;
		bool						_password_set;

		// Mode variables -- solo activos si el modo esta seteado
		int							_channel_limit;
		std::vector<std::string>	_invited_clients;
		std::string					_password;
				
	public:
		Channel(std::string name, Client *client);
		~Channel(void);

		// Modes settings getters and checkers
		bool		isLimitModeSet(void) const;
		bool		isInviteModeSet(void) const;
		bool		isTopicModeSet(void) const;
		bool		isPasswordSet(void) const;
		bool		isPasswordValid(std::string password) const;
		//bool		isClientInvited(Client* Client) const; //puede ser nick

		//is invited


		
		// Variable getters
		std::string	getChannelName(void);

		// Channel functions
		void		addClient(Client *client);
		void		removeClient(Client* client);
		void		addOperator(Client *new_op);
		void		removeOperator(Client *op);
		bool		isClient(Client* client);
		bool		isChannelEmpty(void) const; //llamar cada vez que alguien haga quit or kick
		void		displayTopic(void) const;
		//void broadcastMessage(std::string message);
		//confirm the password;
		

		// Channel functions for ops
		bool		isOperator(std::string nick) const;
		void		changeTopic(std::string topic, Client* client);
		void		kickUser(Client* kicker, Client* target);

		//void	changeMode(Client* client, std::string command);
};


#endif
