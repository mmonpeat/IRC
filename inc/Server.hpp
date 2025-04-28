#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <cstdlib> //EXIT_SUCCESS i EXIT_FALIURE, constants
#include <cstring> //memset...
#include <string>
#include <vector>
#include <map>
#include <unistd.h> //socket, close
#include <sys/socket.h> //bind, listen, accept, send, recv
#include <netdb.h> //getaddrinfo, freeaddrinfo
#include <fcntl.h> //fcntl
#include <poll.h> //poll

#include "Client.hpp"
#include "Channel.hpp"

//class Client;
//class Channel;

class Server
{
	private:
		int		serverPort;
		std::string	serverPass;
		int		serverSocketFd;
	
		std::map<int, Client>		clients;
		std::vector<Channel>		channels;
		std::vector<struct pollfd> 	pollFds;

		//ft_lookup()
		void 	handleNewConnection();
		void	handleClientData(int clientFd);
		void	removeClient(int clientFd);
	public:
		Server(int port, const std::string &pass);
		~Server();

		//void	getServerPort();
		//void	getServerPass();
		
		void	start();
};

#endif
