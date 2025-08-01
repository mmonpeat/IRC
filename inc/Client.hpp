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
		bool		_end_connection;
		std::string	_inputBuffer; //what the server receives from the client
		std::string	_outputBuffer; //what we send to the client

	public:
		Client(int fd);
		~Client(void);


		void		setNick(std::string nick);
		void 		setUserName(std::string user_name);
		void		setRealName(std::string real_name);
		void		setAuth(bool status);
		void		setPass(bool status);
		void		setEnd(bool status);

		std::string	getNick(void) const;
		std::string	getRealName(void) const;
		std::string	getUserName(void) const;
		int			getFd(void) const;
		bool		getAuth(void) const;
		bool		getPass(void) const;
		bool		getEnd(void) const;

		void		addToBuffer(char *buffer);
		bool		readBuffer(std::string *msg);
};

#endif
