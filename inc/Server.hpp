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

#include "Client.hpp"
#include "Channel.hpp"
#include "NumericReplies.hpp"

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
		void			handleMsg(std::string msg, Client* client);
		int				checkCommand(std::string param);
		void			ServerHandshake(std::string *params, Client* client, int command);
		void			CommandCall(std::string *params, Client* client, int command);
		std::string*	returnParams(std::string msg);
		int				countParams(std::string msg);
	
		//command functions
		void	pass(std::string *params, Client* client);		
		void	nick(std::string *params, Client* client);		
		void	user(std::string *params, Client* client);
		
		//reply functions
		void	sendReply(int client_fd, std::string reply);
	
		//handling client
		bool	clientIsRegistered(int clientFd);
		Client*	createClient(int clientFd);
		void	addClient(int clientFd);
		Client*	getClient(int clientFd);

		bool	equalNicks(std::string new_nick, std::string client) const;
		bool 	isNickUnique(std::string nickName) const; // if there are two with the same nickName 433 ERR_NICKNAMEINUSE

		/* Exeptions Classes */
		class specificException: public std::range_error
		{	
			public:
				specificException(const std::string &msg);
		};
};

#endif
