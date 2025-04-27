#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
	private:
		int		password;
		int		fd;
		std::string	nick;
		std::string	user;
		std::string	real_name;
	public:
		Client();
		~Client();
};

#endif
