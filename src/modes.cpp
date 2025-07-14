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
	std::cout << "El Channel con este nombre no existe" << std::endl; //put proper send and num reply
	return false;
}


bool Server::isModeValid(std::string *params) {
	if (!params || params[2].empty())
		return false;

	std::string modeStr = params[2];
	char currentSign = 0;
	bool expectingMode = false;
	size_t found_param = 0;
	size_t max_param_num = 3;

	for (size_t i = 0; i < modeStr.size(); ++i) {
		char c = modeStr[i];

		if (c == '+' || c == '-') {
			if (expectingMode) {
				std::cout << "Error: consecutive '+' or '-' without mode letter" << std::endl;
				return false;
			}
			currentSign = c;
			expectingMode = true;
		}
		else if (c == 'i' || c == 't' || c == 'k' || c == 'l' || c == 'o') {
			if (currentSign == 0) {
				std::cout << "Error: mode character without preceding sign" << std::endl;
				return false;
			}

			expectingMode = false;

			if (c == 'o') {
				// o requires a parameter for both + and -
				++found_param;
			}
			else if ((c == 'k' || c == 'l') && currentSign == '+') {
				// k and l require parameter only on +
				++found_param;
			}

			if (found_param > max_param_num) {
				std::cout << "Error: too many parameter-requiring modes" << std::endl;
				return false;
			}
		}
		else {
			std::cout << "Error: invalid mode character '" << c << "'" << std::endl;
			return false;
		}
	}

	if (expectingMode) {
		std::cout << "Error: mode string ends with a sign" << std::endl;
		return false;
	}

	return true;
}



