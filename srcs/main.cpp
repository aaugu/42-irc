/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:04:56 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/27 11:55:04 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

# define RED  "\e[31m"
# define CYAN "\e[96m"
# define END  "\e[0m"

int	main(int ac, char ** av)
{
	if (ac != 2)
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