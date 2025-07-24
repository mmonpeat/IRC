#include "Server.hpp"

Server::Server(int port, const std::string &pass): serverPort(port), serverPass(pass), serverSocketFd(-1)
{
	init();
	start();
}

Server::~Server()
{
    // Cerrar socket del servidor
    if (serverSocketFd != -1) {
        close(serverSocketFd);
        std::cout << "[Server] Socket closed" << std::endl;
    }

    // Liberar todos los canales
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        delete *it;
    }
    channels.clear();
    
    // Liberar todos los clientes y cerrar sus sockets
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->first != -1) {
            close(it->first); // Cerrar el socket del cliente
        }
        delete it->second;
    }
    clients.clear();

    // Limpiar pollFds
    pollFds.clear();
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
	std::cout << "comenca..." << std::endl;


	struct pollfd serverPoll;
	serverPoll.fd = serverSocketFd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	pollFds.push_back(serverPoll);
	
	while(true)
	{
		int pollCount = poll(pollFds.data(), pollFds.size(), -1); // wait forever
		if (pollCount == -1)
		{
			if (errno == EINTR)
				break ;
			throw Server::specificException("ERROR: poll failed");
		}

		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverSocketFd)
					acceptNewConnection();
				else {
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
}

void Server::handleClientData(int clientFd)
{	
	
	char buffer[512];
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	
	
    if (bytesRead == 0) {
			std::cout << "CONNECTION closed\n";
        	removeClient(clientFd);  // Desconexión o error
	} else if (bytesRead < 0) {
		std::cout << "RECV error\n";
	} else {
     		buffer[bytesRead] = '\0';
			clients[clientFd]->addToBuffer(buffer);

			std::string	msg;
			while (clients[clientFd]->readBuffer(&msg))
			{
				handleMsg(msg, clients[clientFd]);
				mostrarChannels();
			}
		}
}
void Server::mostrarChannels(void)
{
    int i = 1;
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it, ++i)
    {
        Channel* channel = *it;  // Diferencia clave: se usa *it en lugar de it->second
        std::cout << "------------------------------------------------------------------" << std::endl;
        if (!channel) {
			std::cout << "Canal #" << i << " -> Puntero nulo!" << std::endl;
			continue;
		}
		std::cout << "Canal #" << i << " -> Nom: " << channel->getChannelName() << std::endl;
        std::cout << "  Nombre de clients: " << channel->getClientNicks().size() << std::endl;

        if (channel->isPasswordSet())
            std::cout << "  Mode +k (password) activat" << std::endl;
        if (channel->isInviteModeSet())
            std::cout << "  Mode +i (invite-only) activat" << std::endl;
        if (channel->isLimitModeSet())
            std::cout << "  Mode +l (limit) activat. Límits: " << channel->getChannelLimit() << std::endl;

        std::cout << "  Membres: ";
        const std::vector<std::string>& nicks = channel->getClientNicks();
        for (size_t j = 0; j < nicks.size(); ++j)
        {
            std::cout << nicks[j];
            if (j < nicks.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl << std::endl;
    }
}
// void Server::removeClient(int clientFd) 
// {
// 	// Delete pollFds
// 	for (std::vector<struct pollfd>::iterator it = pollFds.begin(); 
// 		it != pollFds.end(); ++it) 
// 	{
// 		if (it->fd == clientFd)
// 		{
// 				pollFds.erase(it);
// 				break;
// 		}
// 	}
// 	std::cout << "Close client with: fd=" << clientFd << std::endl;
// 	delete clients[clientFd];
// 	clients.erase(clientFd);
// 	close(clientFd);
// }
void Server::removeClient(int clientFd)
{
    // 1. Encontrar el cliente
    std::map<int, Client*>::iterator clientIt = clients.find(clientFd);
    if (clientIt == clients.end()) return;

    Client* client = clientIt->second;
	if (!client) return;
    
    // 3. Eliminar cliente de los canales
    for (size_t i = 0; i < channels.size(); ) {
        Channel* channel = channels[i];
        if (channel->isClientInChannel(client)) {
            // Notificar salida
            channel->broadcastMessage(":" + client->getNick() + " QUIT :Client quit\r\n");
            
            // Eliminar cliente
            channel->removeClient(client);
            
            // Eliminar canal si está vacío
            if (channel->isChannelEmpty()) {
                delete channel;
                channels.erase(channels.begin() + i);
                continue; // No incrementar i ya que hemos eliminado un elemento
            }
        }
        ++i;
    }
    
    // 4. Eliminar de pollFds
    for (std::vector<struct pollfd>::iterator it = pollFds.begin(); it != pollFds.end(); ++it) {
        if (it->fd == clientFd) {
            pollFds.erase(it);
            break;
        }
    }
    
    // 5. Cerrar socket y eliminar cliente
    //close(clientFd);
	if (clientFd != -1) {
        close(clientFd);
    }
    delete client;
    clients.erase(clientIt);
}

int		Server::getServerPort( void ) const {
	return (this->serverPort);
}

bool	Server::validPassword( std::string client_pass ) const {
	if (client_pass == this->serverPass)
		return true;
	return false;
}

//------------------------------- Msg Functions ----------------------------------

void	Server::handleMsg(std::string msg, Client *client)
{
	std::cout << "Msg is : " << msg << std::endl;
	if (msg.empty())
		return ;
	
	std::string	*params = returnParams(msg);
	int	command = checkCommand(params[0]);

	if (command == -1)
	{
		std::cout << "unknown command" << std::endl;
		sendReply(client->getFd(), errUnknownCommand(client->getNick(), params[0]));
		delete[] params;
		return ;
	}
	if (client->getAuth() == false)//client is not authorized
	{
		if (command > 3)
		{
			sendReply(client->getFd(), errNotRegistered());
			delete[] params;
			return ;
		}
		ServerHandshake(params, client, command);
	}
	else
		CommandCall(params, client, command);
	delete[] params;
	return ;
}

int	Server::checkCommand(std::string parameter)
{
	std::string	command[10] = 
	{
		"CAP", "PASS", "NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"
	};
	for (int i = 0; i < 10; i++)
	{
		if (parameter.compare(command[i]) == 0)
			return (i);
	}
	return (-1);
}

void	Server::ServerHandshake(std::string *params, Client *client, int command)
{
	switch(command)
	{
		case 0:
			{
				std::string	reply = ":localhost CAP * LS :\r\n";
				sendReply(client->getFd(), reply);
				break ;
			}
		case 1:
			pass(params, client);
			break ;
		case 2:
			if (client->getPass() == true)
				nick(params, client);
				
			break ;
		case 3:
			if (client->getPass() == true && client->getNick().empty() == false)
				user(params, client);
			break ;
		default:
			std::cerr << "Handshake default case, something went wrong" << std::endl;
	}
	return ;
}

void	Server::CommandCall(std::string *params, Client *client, int command)
{
	switch(command)
	{
		case 0:
			{
				std::cout << "CAP END received" << std::endl;
				break ;
			}
		case 1:
			pass(params, client);
			break ;
		case 2:
			nick(params, client); 
			break ;
		case 3:
			user(params, client);
			break ;
		case 4:
			prepareForJoin(params, client);
			break;
		case 5:
			//privmsg(params, client);
			break;
		case 6:
			std::cout << "KICK goes here" << std::endl;
			//kick(params, client);
			break;
		case 7:
			std::cout << "INVITE goes here" << std::endl;
			//invite(params, client);
			break;
		case 8:
			std::cout << "TOPIC goes here" << std::endl;
			//topic(params, client);
			break;
		case 9:
			std::cout << "MODE goes here" << std::endl;
			//mode(params, client);
			break;
		default:
      		sendReply(client->getFd(), errUnknownCommand(client->getNick(), params[0]));
	}
	return ;
}

//Returns an allocated array, delete after use
std::string*	Server::returnParams(std::string msg)
{
	unsigned long	i = 0;
	unsigned long	n = countParams(msg);
	std::string	*params;

	params = new std::string[n];
	std::string::size_type	pos = msg.find(' ');
	while (i < n)
	{
		if (msg[0] == ':')
		{
			params[i] = msg.substr(1, msg.length());
			std::cout << "param " << i << " is " << params[i] << std::endl;
			break ;
		}
		params[i++] = msg.substr(0, pos);
		std::cout << "param " << i - 1 << " is " << params[i - 1] << std::endl;
		msg.erase(0, pos + 1);
		pos = msg.find(' ');
	}
	return (params) ;
}

int	Server::countParams(std::string msg)
{
	int				n = 1;
	unsigned long	i = 0;
	bool			last = false;
	
	while (i < msg.size())
	{
		while (msg[i++] == ' ')
		{
			if (msg[i] != ' ' && !last)
				n++;
			if (msg[i] == ':')
				last = true;
		}
	}
	return (n);
}

//------------------------------- Command Functions ------------------------------

void	Server::pass(std::string *params, Client *client)
{
	std::cout << "do PASS command" << std::endl;
	if (client->getPass() == true)
	{
		sendReply(client->getFd(), errAlreadyRegistered(client->getNick()));
		return ;
	}
	if (params[1].empty())
	{
		std::cout << "TEST" << std::endl;
		sendReply(client->getFd(), errNeedMoreParams(params[0]));
		return ;
	}
	if(this->serverPass.compare(params[1]) == 0)
	{
		std::cout << serverPass << "\n";
		std::cout << params[1] << "\n";
		std::cout << "correct Pass" << std::endl;
		client->setPass(true);
	}
	else
	{
		std::cout << serverPass << "\n";
		std::cout << params[1] << "\n";
		sendReply(client->getFd(), errPassMismatch());
		//close connection?
	}
	return ;
}

void	Server::nick(std::string *params, Client *client)
{
	if (params[1].empty())
	{
		sendReply(client->getFd(), errNoNickNameGiven());
		return ;
	}
	if(isNickUnique(params[1]) == false)
	{
		sendReply(client->getFd(), errNickNameInUse(client->getNick(), params[1]));
		return ;
	}
	if (isNickValid(params[1]) == true)
	{
		client->setNick(params[1]);
		std::cout << "Nick set as " << client->getNick() << std::endl;
	}
	else
		sendReply(client->getFd(), errOneUseNickname());
	return ;
}

void	Server::user(std::string *params, Client *client)
{
	if (params[2].empty())
	{
		sendReply(client->getFd(), errNeedMoreParams(params[0]));
		return ;
	}
	if (client->getAuth() == true)
	{
		sendReply(client->getFd(), errAlreadyRegistered(client->getNick()));
		return ;
	}
	client->setUserName(params[1]);
	std::cout << "user name set as:" << client->getUserName() << "\n";
	client->setRealName(params[4]);
	std::cout << "real name set as: " << client->getRealName() << std::endl;
	client->setAuth(true);
	sendReply(client->getFd(), rplWelcome(client->getNick()));
	std::cout << "auth is " << client->getAuth() << std::endl;
	return ;
}

void	Server::privmsg(std::string *params, Client *client)
{
	std::cout << "PRIVMSG goes here" << std::endl;
	if (params[1].empty())
	{
		sendReply(client->getFd(), errNoRecipient(client->getNick()));
		return ;
	}
	if (params[2].empty())
	{
		sendReply(client->getFd(), errNoTextToSend(client->getNick()));
		return ;
	}
	//check if it's a channel or not
	//	if channel check if client can send to channel
	//separate targets if there're more than one
}

//------------------------------- Reply Functions ------------------------------
void	Server::sendReply(int client_fd, std::string reply)
{
	send(client_fd, reply.c_str(), reply.length(), 0);
	return ;
}

//------------------------------- Client Functions -------------------------------
  
bool Server::clientIsRegistered(int clientFd) {
	std::map<int, Client*>::iterator it = clients.find(clientFd);
	if (it != clients.end()) 
		return true;
	else
		return false;
}


Client*	Server::createClient(int clientFd) {
	Client *new_client = new Client(clientFd);
	return (new_client);
}

void	Server::addClient(int clientFd) {
	
	if (clients.size() >= 10) {
		std::cout << "Server full :(  " << std::endl;
		removeClient(clientFd);
	}
	
	Client *new_client = createClient(clientFd);
	clients.insert(std::make_pair(clientFd, new_client));
	return;
}

Client*	Server::getClient(int clientFd)
{
	std::map<int, Client*>::iterator it = clients.find(clientFd);
	return (it->second);
}

char Server::foldChar(char c) const {
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





