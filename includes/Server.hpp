/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/29 11:24:11 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <netinet/in.h>
# include <vector>
# include <poll.h>
# include <sys/socket.h>

# define MAXCONN (SOMAXCONN + 1)


class Server
{
	private :
		int						nbConnections;
		int						sockfd;
		struct sockaddr_in		addr;
		std::vector<pollfd>		pollFds;

		// socklen_t			_addrlen;

		int run;
		void	setListenBackLog(void);
		void	waitForEvent(void);
		void	acceptNewClient(void);
		void    closePollFds(void);

		Server(void);

	public :
		Server(int port);
		~Server(void);

		void start(void);
		void stop(void);

};

// void	closeClient(Client* client);
// void    deleteClient(Client* client);

#endif
