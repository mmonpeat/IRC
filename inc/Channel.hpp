#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "iostream"
#include <sstream>

// MAX_CHANNELS_PER_CLIENT 10

class Client;

class Channel
{
	private:
		std::string					name;
		std::vector<std::string>	client_nicks;
		std::vector<Client*> 		operators;
		std::vector<Client*> 		clients;//if a client becomes an operator, it's removed from clients and added to operators.
	public:
		Channel();
		~Channel();
		
		int join(Client& client, std::vector<Channel> &channelsExistents, std::vector<std::string> CheckChannels);
};


#endif
