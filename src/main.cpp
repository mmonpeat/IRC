//- 6667 (the default for IRC) to handle incoming clients connections.
//- 0-1023 (privileged ports that usually requiere root and admin privileges)
//- 1024-49151 would be acceptable to avoid conflicts with system-level services
//- avoid (80.443 and 22)

#include "Server.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>

//int	g_status = 0;

int	set_port(char *argv)
{
	std::string	port_av = argv;
	int port = atoi(port_av.c_str());
	return (port);
}

int		check_password(std::string password)
{
	// completar con las reglas acordadas min lenght etc...
	if (password.empty())
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int		check_port(std::string port)
{
	if (port.length() > 5  )
		return EXIT_FAILURE;
	for (size_t i = 0; i < port.length(); i++) {
    	if (!std::isdigit(port[i]))
            return EXIT_FAILURE;
	}
	int prt = atoi(port.c_str());
	if (prt <= 1023 || prt > 49151) 
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int		check_args(int ac, char **av)
{
	if (ac != 3)
		return EXIT_FAILURE;
	std::string port = av[1];
	std::string password = av[2];
	if (check_port(port) == EXIT_FAILURE) {
		std::cout << "port failed" << std::endl;
		return EXIT_FAILURE;
	}
	if (check_password(password) == EXIT_FAILURE) {
		std::cout << "password failed" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void	signal_handler(int signum)
{
	if (signum == SIGTSTP || signum == SIGINT || signum == SIGQUIT)
		std::cout << "\nSignal caught, shutting down...\n";
}

int	start_signals()
{
	struct	sigaction	sa;
	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGTSTP, &sa, NULL) == -1)
	{
		perror("Sigaction failure\n");
		return (EXIT_FAILURE);
	}
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("Sigaction failure\n");
		return (EXIT_FAILURE);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("Sigaction failure\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	if (start_signals() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (check_args(argc, argv) == EXIT_FAILURE) {std::cerr << "Usage: " << argv[0] << " <port>" << std::endl; std::cerr << "User pass: " << argv[1] << " <password>" << std::endl; return (EXIT_FAILURE);}
	int	port = set_port(argv[1]);
	std::string password = argv[2];
	std::cout << port << " y password: " << password << std::endl;
	std::cout << "IRC starts" << std::endl;
	try
	{
		Server server(port, password);
	} catch(const Server::specificException &e) {
		std::cerr << "Server exception caught: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	std::cout << "despres start" << std::endl;
	return (EXIT_SUCCESS);
}

/*
PROVA JOIN
- crear channel1 i channel2
- clients maria, usuari1
- maria esta en channel1 i channel2 i usuari1 en el channel1
- Primera comanda : 
*/

/*
(void)argc;
	(void)argv;
	
	//coses que ja rebre per arguments al join quan tot funcioni
    std::vector<Channel> channelsExistents;

	Client client(1);
	client.setNick("Elna");
	Client Maria(1);
	Maria.setNick("Maria");

	Channel ch1("#Canal1", &client);
	Channel ch2("#Canal2", &client);
	ch1.addClient(&client);
	ch1.addClient(&Maria);
	ch2.addClient(&client);
	channelsExistents.push_back(ch1);
	channelsExistents.push_back(ch2);
	std::cout << "Client està ja en 2 canals.\n";

	//AIXO HA D'ANAR A PARSE JOIN 
	Server serv;
	std::string msg = "&Canal3,#Canal4,#canal1 123,1234,1231";//&Canal3,&Canal4,&Canal5,&Canal6,&Canal7
	std::string *params = serv.returnParams(msg);
	int numParams = serv.countParams(msg);

	if (numParams == 0 || numParams > 3)
		std::cerr << "Error: número invàlid de paràmetres per a JOIN" << std::endl;	
	else
	{
		std::vector<std::string> requestedChannels = serv.convertToVector(params[0]);
		std::vector<std::string> passChannels = serv.convertToVector(params[1]);
		delete[] params;
		
		serv.join(client, channelsExistents, requestedChannels, passChannels);
	}
*/