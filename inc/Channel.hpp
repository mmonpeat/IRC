#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

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

};


#endif


// std::vector<Channel>		channels;
// newChannel

//pot haver-hi múltiples canals en una sola comanda JOIN.(ns pero ua putada)

/*
1. Validar comanda JOIN (canals i keys)
2. Per cada channel
	- comprovaar que el/s nom/s compleix les normes sintàctiques: 
	Ha de començar per & i No pot contenir espais, ^G (ASCII 7), ni comes.
	
	- compovar limit de channels en els que esta el client i en els que ot entrar.
	(si vol entrar mes d'un channel fem n vector/array amb els que vol entrar o 
	simplement el split que rebo de missatge)

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