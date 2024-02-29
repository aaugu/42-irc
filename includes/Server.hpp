/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/27 14:59:12 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <sys/socket.h>

class Server
{
	private :
		int						nbConnections;
		int						_sockfd;
		struct sockaddr_in		_addr;
		struct pollfd			_pollFds[129];

		void	waitForEvent(void);
	public :
		Server(int port);
		~Server(void);

		void start(void);

};
