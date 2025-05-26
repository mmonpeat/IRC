//- 6667 (the default for IRC) to handle incoming clients connections.
//- 0-1023 (privileged ports that usually requiere root and admin privileges)
//- 1024-49151 would be acceptable to avoid conflicts with system-level services
//- avoid (80.443 and 22)

#include "Server.hpp"

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


std::vector<std::string> parseJoinChannels(const std::string& line)
{
	std::vector<std::string> result;
	std::stringstream ss(line);
	std::string channel;

	while (std::getline(ss, channel, ',')) {
		if (!channel.empty())
			result.push_back(channel);
	}
	return result;
}

int main(int argc, char **argv)
{
	// if (check_args(argc, argv) == EXIT_FAILURE) {std::cerr << "Usage: " << argv[0] << " <port>" << std::endl; std::cerr << "User pass: " << argv[1] << " <password>" << std::endl; return (EXIT_FAILURE);}
	// int	port = set_port(argv[1]);
	// std::string password = argv[2];
	// std::cout << port << " y password: " << password << std::endl;
	// std::cout << "IRC starts" << std::endl;
	// try
	// {
	// 	Server server(port, password);
	// } catch(const Server::specificException &e) {
	// 	std::cerr << "Server exception caught: " << e.what() << std::endl;
	// 	return (EXIT_FAILURE);
	// }
	// std::cout << "despres start" << std::endl;

	(void)argc;
	(void)argv;
	
	//coses que ja rebre per arguments al join quan tot funcioni
    std::vector<Channel> channelsExistents;

	Client client(1);
	client.setNick("prova");

	Channel ch1("&Canal1", &client);
	Channel ch2("&Canal2", &client);
	ch1.addClient(&client);
	ch2.addClient(&client);
	channelsExistents.push_back(ch1);
	channelsExistents.push_back(ch2);
	std::cout << "Client està ja en 2 canals.\n";

	std::string joinCommand = "&Canal3,&Canal4,&Canal5,&Canal6,&Canal7";
	std::vector<std::string> requestedChannels = parseJoinChannels(joinCommand);

	Channel fakeJoin("&fake", &client); // només per usar el .join()
	fakeJoin.join(client, channelsExistents, requestedChannels);

	return (EXIT_SUCCESS);
}

/*
//PROVA DE FILTRE DE NOM CHANNEL
    CheckChannels.push_back("&HelloWorld");
	CheckChannels.push_back("#CanalInalid");
	CheckChannels.push_back("&Canal Inalid");
	CheckChannels.push_back("CanalInalid");
	CheckChannels.push_back("&Canal,Invalid");
	CheckChannels.push_back("&TooLong" + std::string(45, 'x'));
*/