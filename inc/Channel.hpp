#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include "Client.hpp"
#include "Server.hpp"

class Client;

class Channel
{
	private:
		std::string					_name;
		std::vector<Client*>		_operators;
		std::vector<Client*>		_clients;
		time_t						_channel_creation_time;

	
		// Mode control 
		bool						_limit_set;
		bool						_invite_set;
		bool						_topic_set;
		bool						_password_set;

		// Mode variables -- solo activos si el modo esta seteado
		int							_channel_limit;
		std::vector<std::string>	_invited_clients;
		std::string					_password;

		// Topic related
		std::string					_topic;
		bool						_topic_init;
		time_t						_topic_creation_time;
		std::string					_who_set_topic;
				
	public:
		Channel(std::string name, Client *client);
		~Channel(void);

		// Modes settings getters and checkers
		bool		isLimitModeSet(void) const;
		bool		isInviteModeSet(void) const;
		bool		isTopicModeSet(void) const;
		bool		isPasswordSet(void) const;
		bool		isPasswordValidChannel(std::string password) const;
		bool		isClientInvited(Client* client) const; //puede ser nick

		// Variable getters
		std::string					getChannelName(void) const;
		std::vector<std::string>	getClientNicks() const;
		int 						getChannelLimit(void) const;
		std::string					getTopic(void) const;
		bool						getTopicInit(void) const;
		size_t 						getClientCount() const { return _clients.size(); }
		std::string					getChannelCreationTime(void);
		std::string					getTopicSetter(void);
		std::string					getTopicSetTime(void);

	
		
		// Channel functions
		void		addClient(Client *client);
		bool 		isClientInChannel(Client* client) const;
		void		removeClient(Client* client);
		void		addOperator(Client *new_op);
		void		addOperatorByNick(const std::string& new_op);
		void		removeOperator(Client *op);
		void		removeOperatorByNick(std::string& ex_op);
		bool		isClient(Client* client);
		bool		isClientByNick(std::string nick);
		bool		isChannelEmpty(void) const; //llamar cada vez que alguien haga quit or kick
		void		displayTopic(void) const;
		void 		broadcastMessage(std::string message) const;
		void		passwordSetBroadcast(Client* client);
		int			numberOfClients(void) const;
		std::string	returnModes(std::string nick);

		// Channel functions for command
		bool		isOperator(std::string nick) const;
		void		changeTopic(std::string topic, Client* client);
		void		kickUser(Client* kicker, Client* target);

		            //mode setters
		void		setPassword(const std::string& password); // preguntar Maria si borrarlo
		void		setPasswordM(Client* op, const std::string& password);
		void		setTopicMode(const std::string& op_nick);
		void		setLimitMode(int limit, const std::string& limit_str, const std::string& op_nick);
		void		setInviteMode(const std::string& op_nick);

		            //mode unsetters
		void		unsetPasswordMode(const std::string& op_nick);
		void		unsetTopicMode(const std::string& op_nick);
		void		unsetLimitMode(const std::string& op_nick);
		void		unsetInviteMode(const std::string& op_nick);


};

#endif