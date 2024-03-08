/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:04:56 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/05 12:05:05 by lvogt            ###   ########.fr       */
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
	for (int i = 0; av[1][i] != '\0'; i++) {
        if (!std::isdigit(av[1][i])) {
            std::cerr << "Port must be a number" << std::endl;
         	return (1);
        }
    }
	try {
		Server	server(atoi(av[1]));
		// std::string password = av[2];
		server.start();
		server.stop();
	}
	catch(const std::exception& e) {
		std::cerr << RED << "Error: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}