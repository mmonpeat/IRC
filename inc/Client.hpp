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
		std::string	_real_name;	// USER params
		bool		_pass;
		bool		_auth;

	public:
		Client(int fd);
		~Client(void);


		void		setNick(std::string nick);
		void 		setUserName(std::string user_name);
		void		setHostName(std::string host_name);
		void		setRealName(std::string real_name);
		void		setPass(bool status);

		std::string	getNick(void) const;
		int			getFd(void) const;
		bool		getAuth(void) const;
		bool		getPass(void) const;
};

#endif
