/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/12 13:18:24 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <vector>
# include <poll.h>
# include <arpa/inet.h>
# include <string>
# include "../includes/Client.hpp"

# define MAXCLIENT 5

# define ERR_SOCK_CREATE	"Could not create socket"
# define ERR_SOCK_OPT		"Could not set socket option"
# define ERR_SOCK_NON_BLOCK	"Could not set sockets to be non blocking"
# define ERR_SOCK_BIND		"Could not bind socket"
# define ERR_SOCK_LISTEN	"Could not listen to the socket"
# define ERR_POLL			"Problem while waiting for fd to perform"
# define ERR_CLIENT_NONEX	"Could not find client with this fd"
# define ERR_CLIENT_ACCEPT	"Could not create connection with client"
# define ERR_CLOSE			"Could not close file descriptor"
# define MAX_CONNECTIONS	"Server cannot accept more client"
# define SERVER_FULL		"Attemped to connect but server is full"

class Server
{
	private :
		int						_nbConnections;
		int						_sockfd;
		struct sockaddr_in		_addr;
		std::vector<pollfd>		_pollFds;
		std::vector<Client> 	_clients;

		// run() sub functions
		void	startServer(void);
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
		int		getLine(int fd, std::string &line);
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