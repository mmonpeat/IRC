/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kate <kate@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 01:07:00 by kate              #+#    #+#             */
/*   Updated: 2025/04/28 01:29:55 by kate             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

//------------------------------ Constructor / Destructors------------------------

Client::Client ( void ) {
	std::cout << "Client constructor called" << std::endl; // mute later
	return;
}

Client::~Client ( void ) {
	std::cout << "Client destructor called" << std::endl; // mute later
	return;
}


//------------------------------ Setters -----------------------------------------

// Maybe not actually needed, maybe full constructor could be used

void	Client::setNick( std::string nick ) {
	this->_nick = nick;
	return;
}
void	Client::setName( std::string name ) {
	this->_name = name;
	return;
}
