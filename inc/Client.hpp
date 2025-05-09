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
		int			_fd;
		std::string	_nick;
		std::string	_username;	// USER params
		std::string	_hostname;	// USER params
		std::string	_real_name;	// USER params

	public:
		Client(int fd);
		~Client(void);

		void		setNick(std::string nick);
		void 		setUserName(std::string user_name);
		void		setHostName(std::string host_name);
		void		setRealName(std::string real_name);

		std::string	getUserName(void) const;
		bool		auth;
		int get_fd() const { return _fd; }
};

#endif
