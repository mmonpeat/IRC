/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kate <kate@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 18:10:45 by kkoval            #+#    #+#             */
/*   Updated: 2025/04/28 00:46:23 by kate             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//------------------------------ Constructor / Destructors------------------------

Server::Server( void ) {
	std::cout << "Server constructor called" << std::endl;
	return;
}

Server::~Server( void ) {
	// !!!delete all clients, channels and everything that has been created!!!
	std::cout << "Server destructor called" << std::endl;
	return;
}


//------------------------------ Setters ----------------------------------------

void	Server::setServerPort( int port ) {
	this->_server_port = port;
	return;
}

void	Server::setServerPass( std::string password ) {
	this->_server_pass = password;
	return;
}


//------------------------------ Getters ----------------------------------------

 

//------------------------------ Functions --------------------------------------

bool	Server::validPassword( std::string client_pass ) const {
	if (client_pass == this->_server_pass)
		return true;
	return false;
}