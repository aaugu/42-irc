/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/22 13:27:48 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <vector>
# include "Client.hpp"


class Server
{
	private :
		int					_sockfd;
		struct sockaddr_in	_addr;
		// socklen_t			_addrlen;
		std::vector<Client*> _clients;

		int run;


		Server(void);

	public :
		Server(int port);
		~Server(void);

		void start(void);
		void stop(void);

};

void	closeClient(Client* client);
void    deleteClient(Client* client);

#endif