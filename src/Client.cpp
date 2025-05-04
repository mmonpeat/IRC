/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoval <kkoval@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:15:25 by kkoval            #+#    #+#             */
/*   Updated: 2025/05/02 13:21:12 by kkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Client.hpp"

//------------------------------ Constructor / Destructors------------------------

Client::Client ( void ): fd(-1) {
	std::cout << "Client constructor called" << std::endl; // mute later
	return;
}

Client::Client(int fd): fd(fd) {}

Client::~Client ( void ) {
	std::cout << "Client destructor called" << std::endl; // mute later
	// delete all stored channels and etc..
	return;
}


//------------------------------ Setters -----------------------------------------

// Maybe not actually needed, maybe full constructor could be used

void	Client::setNick( std::string nick ) {
	this->_nick = nick;
	return;
}
void	Client::setName( std::string name ) {
	this->_real_name = name;
	return;
}

//------------------------------ Functions -----------------------------------------
