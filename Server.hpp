/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kate <kate@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:10:26 by kkoval            #+#    #+#             */
/*   Updated: 2025/04/28 02:18:40 by kate             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h> 
#include <unistd.h>
#include <vector>
#include "Client.hpp"

class   Server
{
    public:
		Server( void );
		~Server( void );
		
		//          Setters
		void	setServerPort( int port );
		void	setServerPass( std::string password );
        void    setServerSocket( int fd );
		
		//          Getters
		int     getServerPort( void ) const;

		//          Functions
		bool	validPassword( std::string client_pass ) const ;
        //checkNick(client);
        //checkPowers(client, channel);

    private:
        int			        _server_port;
        std::string	        _server_pass;
        int			        _socket_fd = -1;
        std::vector<Client> clients;
        //std::vector<Chanel>   channels;
        //ft_lookup() //looks for clients in struct;
};

#endif