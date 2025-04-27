#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstdlib> //EXIT_SUCCESS i EXIT_FALIURE, constants
#include <cstring> //memset...
#include <string>
#include <unistd.h> //socket, close
#include <sys/socket.h> //bind, listen, accept, send, recv
#include <netdb.h> //getaddrinfo, freeaddrinfo
#include <fcntl.h> //fcntl
#include <poll.h> //poll

class Server
{
	private:
		int	serverPort;
		int	serverPass;
		int	serverSocketFd;
	
	public:
		Server(int port, int pass);
		~Server();

		//void	getServerPort();
		//void	getServerPass();
		
		void	start();
};

#endif
