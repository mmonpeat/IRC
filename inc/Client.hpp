#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>

class Server;
class Channel;

class Client
{
	private:
		int			fd;
		std::string	_nick;
		std::string	_username;	// USER params
		std::string	_hostname;	// USER params
		std::string	_real_name;	// USER params

	public:
		Client(void);
		Client(int fd); // en que caso se daria?
		~Client(void);

		void	setNick(std::string nick);
		void 	setName(std::string name);
		int get_fd() const { return fd; }
};

#endif
