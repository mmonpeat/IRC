#include "Server.hpp"

void	Server::channelModes(std::string *params, Client *client) {

	int n = 0;

	while (params[n] != "\0") {
		std::cout << "param is: " << params[n] << std::endl;
		n++;
	}

	for (int i = 0; i < n; i++) {
		if (i == 1)
			if (doesChannelExist(params[1]) == false) {
				std::cout << "This channel does not exist" << std::endl;
				return;
			}
		if (i == 2)
			if (isModeValid(params) == false) {
				std::cout << "Mode parameters are not valid" << std::endl;
				return;
			}
	}
	//si no llega al 2 no habian mode parameter - dar error
	//
	(void)client;
	return;
}


bool	Server::doesChannelExist(std::string& name) {
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++) {
		if (equalNicks(it->getChannelName(), name) == true)
			return true;
	}
	std::cout << "El Channel con este nombre no existe" << std::endl;
	return false;
}

bool	Server::isModeValid(std::string *params) {
	std::cout << "Modes paarameters are :" << params[2] << std::endl;
	return true;
}

