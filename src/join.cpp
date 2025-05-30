#include "Server.hpp"

bool Server::checkChannelNameRules(const std::string& channelName)
{
	// Comprovar longitud
    if (channelName.empty() || channelName.length() > 50) 
	{
        std::cerr << "476 ERR_BADCHANMASK " << channelName << " :Bad Channel Mask" << std::endl;
		return (false);
    }
	// Comprovar comenci amb #
	if (channelName[0] != '#')
	{
		std::cerr << "476 ERR_BADCHANMASK " << channelName << " :Bad Channel Mask" << std::endl;
		return (false);
	}
	//caracters prohibits
	for (size_t j = 0; j < channelName.size(); ++j)
	{
		char c = channelName[j];
		if (c == ' ' || c == ',' || c == 7 || c == '\0' || c == '\r' || c == '\n' || c == ':')
		{
 	       std::cerr << "476 ERR_BADCHANMASK " << channelName << " :Bad Channel Mask" << std::endl;
			return (false);
		}
	}
	return (true);
}

int Server::countClientChannels(Client& client, const std::vector<Channel>& channelsExistents)
{
	int count = 0;
    const std::string& clientNick = client.getNick();
    
    for (std::vector<Channel>::const_iterator it = channelsExistents.begin(); 
         it != channelsExistents.end(); ++it) 
    {
        const std::vector<std::string>& nickList = it->getClientNicks();
        if (std::find(nickList.begin(), nickList.end(), clientNick) != nickList.end()) {
            count++;
        }
    }
    return count;
}

int Server::join(Client& client, std::vector<Channel> &channelsExistents, std::vector<std::string> ChannelsNames, std::vector<std::string> ChannelsPasswords)
{
	const int MAX_CHANNELS_PER_CLIENT = 5;//10

	int currentCount = countClientChannels(client, channelsExistents);
	std::cout << "\nValors de en quants canals esta el client "<< client.getNick() << ":" << currentCount << "\n";
	int slotsLeft = MAX_CHANNELS_PER_CLIENT - currentCount;
	std::cout << "\nNumero de canals als ques es pot afexir/crear:" << currentCount << "\n";

	if (slotsLeft <= 0) {
        std::cerr << "405 ERR_TOOMANYCHANNELS " << client.getNick()
                  << " :You have joined too many channels" << std::endl;
        return -1;
    }
	int validChannelsProcessed = 0; 
	for (size_t i = 0; i < ChannelsNames.size() && slotsLeft > 0; i++)
	{
		const std::string& channelName = ChannelsNames[i];
        if (!checkChannelNameRules(channelName)) {
            continue; // Saltar canals amb noms invàlids
        }
		// Si arribem aquí, el channel és vàlid
        validChannelsProcessed++;
		if (validChannelsProcessed > slotsLeft) {
			std::cerr << "405 ERR_TOOMANYCHANNELS: " << client.getNick()
                      << " " << channelName
                      << " :You have joined too many channels" << std::endl;
            continue;
		}
		if (i <= ChannelsPasswords.size())
		{
			std::string channelPass = ChannelsPasswords[i];
			std::cout << "Processant canal vàlid: " << channelName 
            		<< " amb password: " << channelPass << std::endl;
		}
		std::cout << "Processant canal vàlid: " << channelName << std::endl;
	}
	ChannelsNames.clear();
	ChannelsPasswords.clear();
    return (0);
}

std::vector<std::string> Server::convertToVector(const std::string& line)
{
	std::vector<std::string> result;
	std::stringstream ss(line);
	std::string channel;

	while (std::getline(ss, channel, ',')) {
		if (!channel.empty())
			result.push_back(channel);
	}
	return (result);
}

Server::Server() {
	std::cout << "Server constructor\n";
}

//std::vector<Channel>		channels;
// newChannel

//pot haver-hi múltiples canals en una sola comanda JOIN.i una key per dos channels diferents mirar exemples
/* DOCUMENTACIÓOO
Command Examples:

  JOIN #foobar                    ; join channel #foobar.

  JOIN &foo fubar                 ; join channel &foo using key "fubar".

  JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
                                  and &bar using no key.

  JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
                                  and channel #bar using key "foobar".

  JOIN #foo,#bar                  ; join channels #foo and #bar.


  CLARIFICAAAAAAR
  | Comanda IRC                                 | Canal    | Key        | Relació         |
| ------------------------------------------- | -------- | ---------- | --------------- |
| `JOIN channel1 key1`                        | channel1 | key1       | canal 1 → key 1 |
| `JOIN channel1,channel2 key1,key2`          | channel1 | key1       | canal 1 → key 1 |
|                                             | channel2 | key2       | canal 2 → key 2 |
| `JOIN channel1,channel2 key1`               | channel1 | key1       | canal 1 → key 1 |
|                                             | channel2 | (cap clau) | canal 2 → sense |
| `JOIN channel1,channel2,channel3 key1,key2` | channel1 | key1       | canal 1 → key 1 |
|                                             | channel2 | key2       | canal 2 → key 2 |
|                                             | channel3 | (cap clau) | canal 3 → sense |
| `JOIN channel1,channel2,channel3`           | channel1 | (cap clau) | canal 1 → sense |
|                                             | channel2 | (cap clau) | canal 2 → sense |
|                                             | channel3 | (cap clau) | canal 3 → sense |

*/

/*
1. Validar comanda JOIN (canals i keys)

PER ENTRAR A LA FUNCIO JOIN HA D'ESTAR EN std::vector<std::string> AMB CADA NOM DE NEW CHANNEL
2. Per cada channel
	- comprovaar que el/s nom/s compleix les normes sintàctiques: 
	Ha de començar per &i No pot contenir espais, ^G (ASCII 7), ni comes.
	
	- compovar limit de channels en els que esta el client i en els que pot entrar.
	(si vol entrar mes d'un channel fem n vector/array amb els que vol entrar o 
	simplement el split que rebo de missatge)
	// MAX_CHANNELS_PER_CLIENT
	- si channel amb aquest nom existeix
		- mirar mode: 
			1. si invite-only(te invitacio o error: 473)
				473: ERR_INVITEONLYCHAN "<channel> :Cannot join channel (+i)"
			2. si hi ha lloc i no esta ple +l (Ple: error: 471)
				471: ERR_CHANNELISFULL "<channel> :Cannot join channel (+l)"
			3. si te pass, que el pass sigui correcte (no correcte: error: 475)
				475: ERR_BADCHANNELKEY "<channel> :Cannot join channel (+k)"
				(Dona error al intentar entrar al channel amb un pass errorni.)
				
				Ns en quina situacio pot passar: (Tenim l' error 467 ERR_KEYSET, 
				si se intenta poner pass(+k) en un canal que ya tiene. 
				"<channel> :Channel key already set")

		- tot be 
			1. afegir client al chanel
			2. Informar tots els usuaris del canal amb missatge de JOIN.
			3. Enviar nomes al client:
				- El topic (RPL_TOPIC) si n’hi ha.
					(332: RPL_TOPIC "<channel> :<topic>" -> When sending a TOPIC message to 
					determine the channel topic, one of two replies is sent.  If
					the topic is set, RPL_TOPIC is sent back else
					RPL_NOTOPIC.)

				- La llista d’usuaris (RPL_NAMREPLY i RPL_ENDOFNAMES) documentaci'o:
					353     RPL_NAMREPLY
                        "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
					366     RPL_ENDOFNAMES
									"<channel> :End of /NAMES list"

							- To reply to a NAMES message, a reply pair consisting
							of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
							server back to the client.  If there is no channel
							found as in the query, then only RPL_ENDOFNAMES is
							returned.  The exception to this is when a NAMES
							message is sent with no parameters and all visible
							channels and contents are sent back in a series of
							RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
							the end.
					aaaaaaaaaaaa:
						Command: NAMES
						Parameters: [<channel>{,<channel>}]

						By using the NAMES command, a user can list all nicknames that are
						visible to them on any channel that they can see.  Channel names
						which they can see are those which aren't private (+p) or secret (+s)
						or those which they are actually on.  The <channel> parameter
						specifies which channel(s) to return information about if valid.
						There is no error reply for bad channel names.

						If no <channel> parameter is given, a list of all channels and their
						occupants is returned.  At the end of this list, a list of users who
						are visible but either not on any channel or not on a visible channel
						are listed as being on `channel' "*".

						Numerics:

								RPL_NAMREPLY                    RPL_ENDOFNAMES

						Examples:

						NAMES #twilight_zone,#42        ; list visible users on #twilight_zone
														and #42 if the channels are visible to
														you.

						NAMES                           ; list all visible channels and users
					
	- si no existeix el canal
		- crear channel:
			- afegir client
			- possar el client com operador
			- gestinar pass si s'ha possat  
				461: ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
				(Returned by the server by numerous commands to
				indicate to the client that it didn't supply enough
				parameters.)


Ns en quina situacio pot passar:
462     ERR_ALREADYREGISTRED
		":You may not reregister"

		- Returned by the server to any link which tries to
		change part of the registered details (such as
		password or user details from second USER message).
*/