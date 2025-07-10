#include "Server.hpp"

void	Server::channelModes(std::string *params, Client *client) {

	int i = 0;

	while (params[i] != "\0") {
		std::cout << "param is: " << params[i] << std::endl;
		i++;
	}
	(void)client;
	return;
}
