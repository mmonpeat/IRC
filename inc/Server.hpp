#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <cstdlib> //EXIT_SUCCESS i EXIT_FALIURE, constants
#include <cstring> //memset...
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

#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	private:
		int			serverPort;
		std::string	serverPass;
		int			serverSocketFd;
		
		
	
		std::map<int, Client*>		clients;
		std::vector<Channel>		channels;
		std::vector<struct pollfd> 	pollFds;

		//ft_lookup()
		void 	acceptNewConnection();
		void	handleClientData(int clientFd);
		void	removeClient(int clientFd);
		
		//utils
		bool	isClientAuth(int clientFd); //checks if client has finished handshake

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
		int		handleMsg(std::string msg, int clientFd);
		int		checkCommand(std::string msg);
		std::string*	returnParams(std::string msg);
		int				countParams(std::string msg);
		
		//handeling client
		bool	clientIsRegistered(int clientFd);
		Client*	createClient(int clientFd);
		void	addClient(int clientFd);

		bool	equalNicks(std::string new_nick, std::string client) const;
		bool 	isNickUnique(std::string nickName) const; // if there are two with the same nickName 433 ERR_NICKNAMEINUSE

		/* Exeptions Classes */
		class specificException: public std::range_error
		{	
			public:
				specificException(const std::string &msg);
		};


		/*  JOIN  */
		std::vector<std::string> 	convertToVector(const std::string& line);
		int 						join(Client& client, std::vector<Channel> &channelsExistents, std::vector<std::string> CheckChannels, std::vector<std::string> ChannelsPasswords);
		std::vector<std::string> 	checkChannelNameRules(std::vector<std::string>& CheckChannels);
		int 						countClientChannels(Client& client, const std::vector<Channel>& channelsExistents);
		std::vector<std::string> 	ClientLimitChannels(Client& client, std::vector<Channel>& channelsExistents, std::vector<std::string> newListChannels);
		//default constructor per a provar join, borrrar despres
		Server();
		
};

#endif
