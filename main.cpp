/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kate <kate@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:07:50 by kkoval            #+#    #+#             */
/*   Updated: 2025/04/28 03:06:05 by kate             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc_lib.hpp"

//- 6667 (the default for IRC) to handle incoming clients connections.
//- 0-1023 (privileged ports that usually requiere root and admin privileges)
//- 1024-49151 would be acceptable to avoid conflicts with system-level services
//- avoid (80.443 and 22)

void	set_server_parameters(Server *server, char **av)
{
	std::string	port_av = av[1];
	int port = atoi(port_av.c_str());
	std::string	password = av[2];
	
	server->setServerPort(port);
	server->setServerPass(password);
}

int		check_password(std::string password)
{
	if (password.empty())
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int		check_port(std::string port)
{
	if (port.length() > 5  )
		return EXIT_FAILURE;
	for (size_t i = 0; i < port.length(); i++) {
    	if (!std::isdigit(port[i]))
            return EXIT_FAILURE;
	}
	int prt = atoi(port.c_str());
	if (prt <= 1023 || prt > 49151) 
		return EXIT_FAILURE;
	// si es disponible y si no fuera
	return EXIT_SUCCESS;
}

int		check_args(int ac, char **av)
{
	if (ac != 3)
		return EXIT_FAILURE;
	std::string port = av[1];
	std::string password = av[2];
	if (check_port(port) == EXIT_FAILURE) {
		std::cout << "port failed" << std::endl;
		return EXIT_FAILURE;
	}
	if (check_password(password) == EXIT_FAILURE) {
		std::cout << "password failed" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int	main(int ac, char **av)
{
	Server	server;
	int		server_socket;
	
	if (check_args(ac, av) == EXIT_FAILURE)
		return EXIT_FAILURE;
	set_server_parameters(&server, av);
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket = -1)
		return EXIT_FAILURE;
	//bind socket to port
	
	//listening loop
	close(server_socket); // socket needs to be closed 
	return EXIT_SUCCESS;
}