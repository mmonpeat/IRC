#include "Server.hpp"

bool	Server::isNickValid(std::string nick)
{
	// must not have: ',' '*' '?' '!' '@' also should not '.'
	std::string	forbidden[6] = {",", "*" , "?", "!", "@", "."};
	for (int i = 0; i < 6; i++)
	{
		std::cout << forbidden[i] << std::endl;
		if (nick.find(forbidden[i]) != std::string::npos)
		{
			std::cout << "oops found :" << forbidden[i] << std::endl;
			return(false);
		}
	}
	//must not start with: '$' or ':' or '#'
	if (nick[0] == '$' || nick[0] == ':' || nick[0] == '#')
		return (false);
	return (true);
}
