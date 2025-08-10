#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <cstdlib> //EXIT_SUCCESS i EXIT_FALIURE, constants
#include <cstring> //memset...
#include <cerrno> //errno
#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <unistd.h> //socket, close
#include <sys/socket.h> //bind, listen, accept, send, recv
#include <netdb.h> //getaddrinfo, freeaddrinfo
#include <fcntl.h> //fcntl
#include <poll.h> //poll
#include <sstream>
#include <algorithm>//std::find

#include "Client.hpp"
#include "Channel.hpp"
#include "NumericReplies.hpp"

class Server
{
	private:
		int			serverPort;
		std::string	serverPass;
		int			serverSocketFd;
		
		
	
		std::map<int, Client*>			clients;
		std::vector<Channel*>			channels;
		std::vector<struct pollfd> 		pollFds;

		void 	acceptNewConnection();
		void	handleClientData(int clientFd);
		void	removeClient(int clientFd);
	
	public:
		Server(int port, const std::string &pass);
		~Server();

		int		getServerPort() const;
		bool	validPassword( std::string client_pass ) const;
		void	init();
		void	initializeSocket();
		void	setupAddress(sockaddr_in &addr);
		void	bindAndListen(sockaddr_in &addr);
		void	start();

		//parser msg functions
		void						handleMsg(std::string msg, Client* client);
		int							checkCommand(std::string param);
		void						ServerHandshake(std::vector<std::string> params, Client* client, int command);
		void						CommandCall(std::vector<std::string> params, Client* client, int command);
		std::vector<std::string>	returnParams(std::string msg);
	
		//command functions
		void	pass(std::vector<std::string> params, Client* client);		
		void	nick(std::vector<std::string> params, Client* client);		
		void	user(std::vector<std::string> params, Client* client);
		void	privmsg(std::vector<std::string> params, Client* client);
		void	privmsg_channel(Client *sender, std::string target, std::string msg);
		void	privmsg_user(Client *sender, std::string target, std::string msg);
		void	quit(Client* client);

		//reply functions
		void	sendReply(int client_fd, std::string reply);
		void	ServerBroadcast(std::string msg);
	
		//handling client
		bool	clientIsRegistered(int clientFd);
		Client*	createClient(int clientFd);
		void	addClient(int clientFd);
		Client*	getClient(int clientFd);

		bool 	isNickUnique(std::string nickName) const; // if there are two with the same nickName 433 ERR_NICKNAMEINUSE

		/* Exeptions Classes */
		class specificException: public std::range_error
		{	
			public:
				specificException(const std::string &msg);
		};

		//other commands utils
		bool		isNickValid(std::string nick);
		int			findClient(std::string nick); //returns fd
		Channel*	findChannel(std::string channel_name); //returns Channel
		Client* 	findClientByNick(const std::string& nick);

		/*  MODE  */
		void						channelModes(std::vector<std::string> params, Client *client);
		int							strToInt(std::string arg); 
		bool						isLimitValid(const std::string& limit);
		void						applyModes(std::vector<std::string> params, Client *client, Channel* channel);
		void						execMode(char sign, char c, std::string param, Client* client, Channel *channel);
		void						modeK(Channel *channel, std::string password, char sign, Client *client);
		void						modeT(Channel *channel, char sign, Client *client);
		void						modeL(Channel *channel, std::string arg, char sign, Client *client);
		void						modeO(Channel *channel, std::string arg, char sign, Client *client);
		void						modeI(Channel *channel, char sign, Client *client);

		/*  TOPIC  */
		Channel*					getChannelByName(std::string& name);
		void						topic(std::vector<std::string> params, Client *client);
		void						showTopic(Channel* channel, Client* client);
		void						setTopic(Channel *channel, Client *client, const std::string& topic);

		/*  KICK  */
		void						kick(std::vector<std::string> params, Client *client);

		/*  INVITE  */
		void						invite(std::vector<std::string> params, Client *client);


		/*  JOIN  */
		int 						join(Client& client, std::vector<Channel*>&channelsExistents, std::vector<std::string> CheckChannels, std::vector<std::string> ChannelsPasswords);
		bool					 	checkChannelNameRules(Client& client, const std::string& channelName);
		int 						countClientChannels(Client& client, const std::vector<Channel*>& channelsExistents);
		void						checkModeToAddClient(Client& client, std::vector<Channel*>& channelsExistents, std::string& channelName, std::string& channelPass);
		void						createNewChannel(Client& client, std::vector<Channel*>& channelsExistents, const std::string& channelName, const std::string& channelPass);
		
		//utils join
		void						prepareForJoin(std::vector<std::string> params, Client *client);
		std::vector<std::string> 	convertToVector(const std::string& line);
		bool						equalChannels(std::string new_channel, std::string channel) const;
		bool						isChannelNameUnique(std::string& channelToCheck, const std::vector<Channel*>& channelsExistents) const;
		std::string					getUniqueChannelName(std::string& channelToCheck, const std::vector<Channel*>& channelsExistents) const;
		

		void 						replayMsgBecauseClientAddedToChannel(Channel* channel, Client& client, std::string channelName);

		//per mostrar borrar
		void 	mostrarChannels(void);
		
};

char	foldChar(char c);
bool	equalNicks(std::string new_nick, std::string client);

#endif
