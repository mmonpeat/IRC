#include "Server.hpp"

Server::Server(int port, const std::string &pass): serverPort(port), serverPass(pass), serverSocketFd(-1)
{
	serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd == -1) //ERROR = -1, .hpp
		throw Server::specificException("ERROR: the socket does not work :(" );
	sockaddr_in addr;// amb .hpp ?
	memset(&addr, 0, sizeof(addr)); //all 0
				
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; //Accept any addres
	addr.sin_port = htons(serverPort);

	if (bind(serverSocketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)//ERROR
	{
		close(serverSocketFd);
		throw Server::specificException("ERROR: Failed to bind socket" );
	}
	if (listen(serverSocketFd, SOMAXCONN) == -1)//Escolta el maxim possible i ERROR  
    	{
		close(serverSocketFd);
		throw Server::specificException("ERROR: Failed to listen on socket" );
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
	

	struct pollfd serverPoll;
	serverPoll.fd = serverSocketFd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	pollFds.push_back(serverPoll);
	std::cout << "Principal loop" << std::endl;
	
	while(true)
	{
		int pollCount = poll(pollFds.data(), pollFds.size(), -1); // wait forever
		if (pollCount == -1)
		{
			std::cerr << "ERROR in poll() " << std::endl;
			break;
		}

		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverSocketFd)
				{
					handleNewConnection();
				} else {
					handleClientData(pollFds[i].fd);
				}
			}
		}
	}
}

void Server::handleNewConnection() 
{
	sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);
	int clientFd = accept(serverSocketFd, (sockaddr*)&clientAddr, &clientLen);

	if (clientFd == -1) 
	{
		std::cerr << "Error en accept()" << std::endl;
        	return;
	}

	fcntl(clientFd, F_SETFL, O_NONBLOCK); // Non-blocking
	// Add poll
	struct pollfd clientPoll;
	clientPoll.fd = clientFd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;
	pollFds.push_back(clientPoll);

	clients.insert(std::make_pair(clientFd, Client(clientFd))); // Create client
	std::cout << "New client: fd=" << clientFd << std::endl;
}


void Server::handleClientData(int clientFd)
{
	char buffer[512];
    	int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    	if (bytesRead <= 0) {
        	removeClient(clientFd);  // Desconexión o error
	} else {
     		buffer[bytesRead] = '\0';
        	std::cout << "Mensaje de fd=" << clientFd << ": " << buffer << std::endl;
        	// Aquí procesarías el comando IRC (ej: /join, /nick, etc.)
        	send(clientFd, buffer, bytesRead, 0);  // Echo (para prueba)
    	}
}

void Server::removeClient(int clientFd) 
{
    
    	// Delete pollFds
    	for (std::vector<struct pollfd>::iterator it = pollFds.begin(); 
			it != pollFds.end(); ++it) 
	{
        	if (it->fd == clientFd) 
		{
            		pollFds.erase(it);
            		break;
        	}
    	}
    	std::cout << "Close client with: fd=" << clientFd << std::endl;
    	clients.erase(clientFd);
	close(clientFd);
}


Server::specificException::specificException(const std::string &msg): std::range_error(msg) {}





