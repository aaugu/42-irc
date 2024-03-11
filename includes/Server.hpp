/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/11 14:40:33 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <vector>
# include <poll.h>
# include <arpa/inet.h>
# include <string>

# define MAXCLIENT 5

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
		void	getClientInput(std::vector<pollfd>::iterator clientPollFd, std::string& clientInput);
		void	executeClientInput(std::string clientInput, int sockfdClient);

		// Client Utils
		int		acceptNewClient(void);
		void	refuseClient(int sockfdClient);
		void	addClientToListenPoll(int sockfdClient);
		void	disconnectClient(std::vector<pollfd>::iterator pollfd);

		// Input utils
		// std::string	checkCapFlags(char* buffer, int sockfdClient);

		// Stop utils
		void    closePollFds(void);

		// Accessors
		std::vector<Client>::iterator	getClientByFd(int sockfdClient);

	public :
		Server(int port);
		~Server(void);

		void run(void);
		void stop(void);

		// DEBUG
		void printNickname();
};

#endif