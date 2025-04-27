/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kate <kate@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 01:07:04 by kate              #+#    #+#             */
/*   Updated: 2025/04/28 01:33:08 by kate             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class   Client
{
    public:
		Client( void );
		~Client( void );

		void	setNick( std::string nick );
		void	setName( std::string name );

    private:
        std::string	_nick;
		std::string	_name;
        //int			_fd;
};

#endif