#include "Server.hpp"

Server::Server(int port, const std::string &pass): serverPort(port), serverPass(pass), serverSocketFd(-1)
{
	init();
	start();
}

Server::~Server()
{
	close(serverSocketFd);
	// delete all clients if allocated
	// delete all channels if allocated
	std::cout << "[Server] Socket closed" << std::endl;

}

void Server::init()
{
	std::cout << "entra init" << std::endl;
	initializeSocket();

	sockaddr_in addr;
    setupAddress(addr);
    bindAndListen(addr);
}

void Server::initializeSocket() 
{
    serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd == -1) //ERROR = -1, .hpp
		throw Server::specificException("ERROR: the socket does not work :(" );
	//obtenemos las flags del socket actual
	int flags = fcntl(serverSocketFd, F_GETFL, 0);
	if (flags == -1) 
	{
		close(serverSocketFd);
		throw Server::specificException("ERROR: fcntl F_GETFL failed");
	}
	//lo ponemos en non-blocking
	if (fcntl(serverSocketFd, F_SETFL, flags | O_NONBLOCK) == -1) 
	{
		close(serverSocketFd);
		throw Server::specificException("ERROR: fcntl F_SETFL O_NONBLOCK failed");
	}
	// Opcional: Reutilizar dirección para evitar errores "Address already in use"
	int opt = 1;
	if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(serverSocketFd);
		throw Server::specificException("ERROR: setsockopt SO_REUSEADDR failed");
	}
	// Evitar crash del servidor si un cliente se desconecta mientras escribes: (ns si va aqui)
	// signal(SIGPIPE, SIG_IGN);
}

void Server::setupAddress(sockaddr_in &addr)
{
	memset(&addr, 0, sizeof(addr)); //all 0
				
	addr.sin_family = AF_INET;//IPv4
	addr.sin_addr.s_addr = INADDR_ANY; //Accept any addres
	addr.sin_port = htons(serverPort);//convertimos el puerto de bits a network(xarxa)
}

void Server::bindAndListen(sockaddr_in &addr) 
{
	if (bind(serverSocketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)//ERROR
	{
		close(serverSocketFd);
		throw Server::specificException("ERROR: Failed incorrect port: already in use or you don't have access(permisos)" );
	}
    if (listen(serverSocketFd, SOMAXCONN) == -1)//Escolta el maxim possible i ERROR  
    {
		close(serverSocketFd);
		throw Server::specificException("ERROR: Failed to listen on socket" );
	}
}


void Server::start()
{
	std::cout << "comencar a acceptar, send, recv, fer poll..." << std::endl;


	struct pollfd serverPoll;
	serverPoll.fd = serverSocketFd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	pollFds.push_back(serverPoll);
	std::cout << "Principal loop poll" << std::endl;
	
	while(true)
	{
		int pollCount = poll(pollFds.data(), pollFds.size(), -1); // wait forever
		if (pollCount == -1)
		{
			throw Server::specificException("ERROR: poll failed");
		}

		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			std::cout << "dins for" << std::endl;
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverSocketFd)
				{
					acceptNewConnection();
					std::cout << "loop handle new connection" << std::endl;
				} else {
					std::cout << "loop handle client data" << std::endl;
					if (clientIsRegistered(pollFds[i].fd) == false)
						addClient(pollFds[i].fd);
					handleClientData(pollFds[i].fd);
				}
			}
		}
	}
}

void Server::acceptNewConnection() 
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

	//std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) 
           //   << ":" << ntohs(clientAddr.sin_port) << std::endl;
	//clients.insert(std::make_pair(clientFd, Client(clientFd))); // Create client
	//std::cout << "New client: fd=" << clientFd << std::endl;
}

void	Server::handleMsg(std::string msg, int clientFd)
{
	std::cout << "Msg is : " << msg << std::endl;
	std::cout << "fd is : " << clientFd << std::endl;
	if (isAuth(clientFd) == false)//client is not authorized
	{
		std::cout << "Handshake goes here" << std::endl;
	}
	std::cout << "After handshake" << std::endl;
	//parse command
}

void Server::handleClientData(int clientFd)
{	
	
	char buffer[512];
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	

    if (bytesRead <= 0) {
        	removeClient(clientFd);  // Desconexión o error
	} else {
     		buffer[bytesRead] = '\0';
			std::string	received(buffer);
			std::string	del = "\r\n";
			std::string::size_type	pos = received.find(del);
			while (pos != std::string::npos)
			{
				handleMsg(received.substr(0, pos), clientFd);
				received.erase(0, pos + del.length());
				pos = received.find(del);
			}
			std::cout << "Mensaje de fd=" << clientFd << ": " << buffer << std::endl;
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

int		Server::getServerPort( void ) const {
	return (this->serverPort);
}

bool	Server::validPassword( std::string client_pass ) const {
	if (client_pass == this->serverPass)
		return true;
	return false;
}

//------------------------------- Client Functions -------------------------------

bool Server::clientIsRegistered(int clientFd) {
	std::map<int, Client*>::iterator it = clients.find(clientFd);
	if (it != clients.end()) {
		// Found the client
		std::cout << "Client with fd " << clientFd << " found: ";
		std::cout << it->second->getNick() << std::endl;
		return true;
	} else {
		std::cout << "No client found for fd: " << clientFd << std::endl;
		return false;
	}
}


Client*	Server::createClient(int clientFd) {
	Client *new_client = new Client(clientFd);
	return (new_client);
}

void	Server::addClient(int clientFd) {
	
	if (clients.size() >= 10) {
		std::cout << "Server full :(  " << std::endl;
		// habra que eliminar este fd y cerrar la conexion
	}
	
	Client *new_client = createClient(clientFd);
	clients.insert(std::make_pair(clientFd, new_client));
	return;
}

char foldChar(char c) {
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    if (c == '[') return '{';
    if (c == ']') return '}';
    if (c == '\\') return '|';
    if (c == '^') return '~';
    return c;
}

bool	Server::equalNicks(std::string new_nick, std::string client) const {
	if (new_nick == client)
		return true;
	if (new_nick.size() != client.size())
		return false;
	for (size_t i = 0; i < client.size(); i++) {
		if (foldChar(new_nick[i]) != foldChar(client[i]))
			return false;
	}
	return true;
}

bool	Server::isNickUnique(std::string nickToCheck) const {
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        Client* client = it->second;
        if (client && client->getAuth()) {
			if (equalNicks(client->getNick(), nickToCheck))
				return false;
		}
	}
    return true;
}

Server::specificException::specificException(const std::string &msg): std::range_error(msg) {}





