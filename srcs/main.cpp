/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:04:56 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/29 12:21:12 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include "../includes/Server.hpp"

# define RED  "\e[31m"
# define CYAN "\e[96m"
# define END  "\e[0m"

int	main(int ac, char ** av)
{
	if (ac != 2) // change to 3 when password is implemented
	{
		std::cout << CYAN "Usage: ./ircserv <port> <password>" END << std::endl;
		return (1);
	}

	try {
		Server	server(atoi(av[1]));

		server.start();
		// server.stop();
	}
	catch(const std::exception& e) {
		std::cerr << RED << "Error: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
