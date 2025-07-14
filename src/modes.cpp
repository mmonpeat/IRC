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


bool	Server::isModeValid(std::string *params)
{
	std::cout << "Modes parameters are: " << params[2] << std::endl;

	const size_t max_param_num = 3;
	size_t found_param = 0;
	char currentSign = 0;

	std::string validModes = "itkol";
	for (size_t i = 0; i < params[2].size(); ++i)
	{
		char c = params[2][i];

		// Handle +/-
		if (c == '+' || c == '-')
		{
			currentSign = c;
			continue;
		}

		// Check for invalid mode characters
		if (validModes.find(c) == std::string::npos)
		{
			std::cout << "Error: invalid mode character '" << c << "'" << std::endl;
			return false;
		}

		// RFC-compliant: count param if mode needs one
		// ---------------- RFC logic below ----------------
		if ((c == 'k' && (currentSign == '+' || currentSign == '-')) || // highlight: RFC requires parameter for -k
			(c == 'l' && currentSign == '+') ||
			(c == 'o') // both +o and -o need a param
		)
		{
			found_param++;
			if (found_param > max_param_num)
			{
				std::cout << "Error: too many parameter-requiring modes" << std::endl;
				return false;
			}
		}
		// ---------------- end RFC logic ------------------
	}

	return true;
}


// LIMIT 3 modes that require parameter
/* input /MODE #pepe +k+l-t+i+o koko 8 poter Polly2 
 no msg for too many parameters but only executes 3 modes that requiere parameters
	output: for all users of the channel
* Polly sets mode +i on #pepe
* Polly sets mode -t on #pepe
* Polly sets channel keyword to koko
* Polly sets channel limit to 8 */

/* Faulty parameters 



*/

// MODE K -k requieres a password to remove the mode 
/*
	Polly2 sets channel keyword to JOhny
	Polly2 removes channel keyword
*/