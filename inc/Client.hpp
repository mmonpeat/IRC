#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Server;
class Channel;

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
		Client(int fd);
		~Client();
};

#endif
