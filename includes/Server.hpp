/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/23 14:16:01 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <vector>
# include <poll.h>
# include <sys/socket.h>


class Server
{
	private :
		int						nbConnections;
		int						_sockfd;
		struct sockaddr_in		_addr;
		std::vector<pollfd>*	_pollFds;

		// socklen_t			_addrlen;

		int run;

		void	waitForEvent(void);

		Server(void);

	public :
		Server(int port);
		~Server(void);

		void start(void);
		void stop(void);

};

void    closePollFd(pollfd& pollFd);
// void	closeClient(Client* client);
// void    deleteClient(Client* client);

#endif