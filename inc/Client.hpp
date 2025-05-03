/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoval <kkoval@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:18:11 by kkoval            #+#    #+#             */
/*   Updated: 2025/05/02 13:19:18 by kkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>

class Server;
class Channel;

class Client
{
	private:
		int			fd;
		std::string	_nick;
		std::string	_user;
		std::string	_real_name;

	public:
		Client(void);
		Client(int fd); // en que caso se daria?
		~Client(void);

		void	setNick(std::string nick);
		void 	setName(std::string name);
		int get_fd() const { return fd; }
};

#endif
