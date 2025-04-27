/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kate <kate@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:10:26 by kkoval            #+#    #+#             */
/*   Updated: 2025/04/28 01:32:21 by kate             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
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
		
		//          Getters
		

		//          Functions
		bool	validPassword( std::string client_pass ) const ;
        //checkNick(client);
        //checkPowers(client, channel);

    private:
        int			        _server_port;
        std::string	        _server_pass;
        //int			        _server_socket_fd;
        std::vector<Client> clients;
        //std::vector<Chanel>   channels;
        //ft_lookup() //looks for clients in struct;
};

#endif