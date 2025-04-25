/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoval <kkoval@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:10:26 by kkoval            #+#    #+#             */
/*   Updated: 2025/04/25 19:14:21 by kkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
//#include "Client.hpp" incluir cliente una vez este

class   Server
{
    public:
		Server( void );
		~Server( void );
		
		//----------Setters
		void	setServerPort( int port );
		void	setServerPass( std::string password );
		
		//----------Getters
		//getServerPort();
		//getServerPass();

		//----------Functions
		//bool	checkPassword()  !!what is the arg?
        //checkNick(client);
        //checkPowers(client, channel);
    private:
        int			_server_port;
        std::string	_server_pass;
        int			_server_socket_fd;
        //std::vector	clients;
        //std::vector	channels;
        //ft_lookup() //looks for clients in struct;
};

#endif