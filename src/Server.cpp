#include "Server.hpp"

Server::Server(int port, int pass): serverPort(port), serverPass(pass)
{
	serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd == -1) //ERROR = -1, .hpp
	{
		std::cerr << "ERROR: socket" << std::endl; //throw error
		exit(EXIT_FAILURE);
	}
	sockaddr_in addr;// amb .hpp ?
	memset(&addr, 0, sizeof(addr)); //all 0
				
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; //Accept any addres
	addr.sin_port = htons(serverPort);

	if (bind(serverSocketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)//ERROR
	{
		close(serverSocketFd);
		std::cerr << "Failed to bind socket" << std::endl; // throw error
		exit(EXIT_FAILURE);
	}
	if (listen(serverSocketFd, SOMAXCONN) == -1)//Escolta el maxim possible i ERROR  
    	{
		close(serverSocketFd);
		std::cerr << "Failed to listen on socket" << std::endl; //throw error
		exit(EXIT_FAILURE);
	}
	std::cout << "[Server] Listening on port" << std::endl;
	
}

Server::~Server()
{
	close(serverSocketFd);
	std::cout << "[Server] Socket closed" << std::endl;

}

void Server::start()
{
	std::cout << "comencar a acceptar, send, recv, fer poll..." << std::endl;
	fcntl(serverSocketFd, F_SETFL, O_NONBLOCK);//socket non-blocking
	
	//map<int, Client> clients ??
	pollfd serverPollFd;

}
