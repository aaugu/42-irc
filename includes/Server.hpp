/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/20 14:34:06 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <stdlib.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>

# include <list>
# include "Client.hpp"


class Server
{
	private :
		int					_socket;
		struct sockaddr_in	_addr;
		std::list<Client*>	_clients;

		int run;


		Server(void);

	public :
		Server(int port);
		~Server();

		void start(void);
		void stop(void);

};

void	closeClient(Client* client);
void    deleteClient(Client* client);

#endif