#include "Server.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	//if (argc != 3) { std::cerr << "Usage: " << argv[0] << " <port>" << std::endl; std::cerr << "User pass: " << argv[1] << " <password>" << std::endl; return (EXIT_FELIURE);}
	
	std::cout << "IRC starts" << std::endl;
	
	int port = 8080;
	int pass = 123456789;
	Server server(port, pass);
	server.start();

	std::cout << "despres start" << std::endl;

	return (EXIT_SUCCESS);
}
