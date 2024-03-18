/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/14 15:47:42 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <netinet/in.h>
# include <vector>
# include <poll.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <string.h>
# include <poll.h>
# include "../includes/Client.hpp"

class Client;

class Server
{
	private :
		int						_nbConnections;
		std::string				_password;
		int						_sockfd;
		struct sockaddr_in		_addr;
		std::vector<pollfd>		_pollFds;
		std::vector<Client> 	_clients;

		// Start() sub functions
		void	startServer(void);
		void	waitForEvent(void);
		void	createClientConnection(void);
		void	getClientInput(std::vector<pollfd>::iterator clientPollFd, std::string& clientInput);
		void	parseClientInput(std::string clientInput, int sockfdClient);
		void	executeClientInput(Server &server, std::vector<pollfd>::iterator pollfd);

		// Client Utils
		int		acceptNewClient(void);
		void	refuseClient(int sockfdClient);
		void	addClientToListenPoll(int sockfdClient);

		// Input utils
		int		getLine(int fd, std::string &line);

		// Destructor utils
		void    closePollFds(void);

		// Accessors
		std::vector<Client>::iterator	getClientByFd(int sockfdClient);

	public :
		Server(int port, std::string password);
		~Server(void);

		void run(void);
		void stop(void);
		
		std::vector<std::string>	getNicknameList();
		std::string					get_password() const;
		void		disconnectClient(std::vector<pollfd>::iterator pollfd);
		// DEBUG
		void printNickname();
};

#endif