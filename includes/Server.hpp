/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/07 13:53:33 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <netinet/in.h>
# include <vector>
# include <poll.h>
# include <sys/socket.h>
# include <string>
# include <poll.h>
# include "../includes/Client.hpp"

class Client;

class Server
{
	private :
		int						_nbConnections;
		int						_sockfd;
		struct sockaddr_in		_addr;
		std::vector<pollfd>		_pollFds;
		std::vector<Client> 	_clients;
		// int run;

		// run() sub functions
		void	setListenBackLog(void);
		void	waitForEvent(void);
		void	createClientConnection(void);
		void	handleClientInput(std::vector<pollfd>::iterator clientPollFd);

		// Client Utils
		int		acceptNewClient(void);
		void	addClientToListenPoll(int sockfdClient);
		void	disconnectClient(std::vector<pollfd>::iterator pollfd);

		// Input utils
		// std::string	checkCapFlags(char* buffer, int sockfdClient);

		// Destructor utils
		void    closePollFds(void);

		// Accessors
		std::vector<Client>::iterator	getClientByFd(int sockfdClient);

		// Server(void);

	public :
		Server(int port);
		~Server(void);

		void run(void);
		void stop(void);
};

// std::string t(const std::string& input);

#endif