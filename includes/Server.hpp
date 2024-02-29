/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/29 20:57:57 by aaugu            ###   ########.fr       */
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
		int run;

		// Start() sub functions
		void	setListenBackLog(void);
		void	waitForEvent(void);
		void	acceptNewClient(void);
		void	checkDisconnectClient(void);
		void	getClientInput(std::string& clientInput, int* sockfdClient);
		void	executeClientInput(std::string clientInput, int fd);
		void    closePollFds(void);

		Server(void);

	public :
		Server(int port);
		~Server(void);

		void start(void);
		void stop(void);

};

#endif
