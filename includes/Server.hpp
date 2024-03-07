/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/01 14:08:11 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <netinet/in.h>
# include <vector>
# include <poll.h>
# include <sys/socket.h>
# include <string>
# include <iostream>
# include <list>
# include <sstream>
# include <arpa/inet.h>
# include <fcntl.h>
# include <err.h>
# include <cstring>
# include <unistd.h>
# include <algorithm>
# include <fcntl.h>
# include <errno.h>

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
		int run;

		// Start() sub functions
		void	setListenBackLog(void);
		void	waitForEvent(void);
		void	addNewClient(void);
		void	getClientInput(std::string& clientInput, int* sockfdClient);
		void	executeClientInput(std::string clientInput, int fd);

		// Client Utils
		int		acceptNewClient(void);
		void	addClientToListenPoll(int sockfdClient);
		void	disconnectClient(std::vector<pollfd>::iterator pollfd, std::vector<Client>::iterator client);

		// Input utils
		std::string	checkCapFlags(char* buffer, int sockfdClient);

		// Destructor utils
		void    closePollFds(void);

		Server(void);

	public :
		Server(int port);
		~Server(void);

		void start(void);
		void stop(void);

		void printNickname();
};

// std::string t(const std::string& input);

#endif