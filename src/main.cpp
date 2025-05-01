#include "Server.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	//if (argc != 3) { std::cerr << "Usage: " << argv[0] << " <port>" << std::endl; std::cerr << "User pass: " << argv[1] << " <password>" << std::endl; return (EXIT_FELIURE);}
	
	std::cout << "IRC starts" << std::endl;
	
	int port = 6698;
	std::string pass = "1234";
	try
	{
		Server server(port, pass);
	} catch(const Server::specificException &e) {
		std::cerr << "Server exception caught: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	std::cout << "despres start" << std::endl;

	return (EXIT_SUCCESS);
}

