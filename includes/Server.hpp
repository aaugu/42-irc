/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/15 16:25:46 by aaugu            ###   ########.fr       */
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
# include "../includes/Channel.hpp"

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
		std::vector<Channel>	_channels;

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
		
		// Accessors
		std::vector<std::string>	getNicknameList(void);
		std::string					get_password(void) const;
		std::vector<Channel>		getChannels(void);
		
		// Channel utils
		void							addChannel(Channel& channel);
		void							removeChannel(std::vector<Channel>::iterator channel);
		std::vector<Channel>::iterator	getChannelByName(std::string name);

		
		void	disconnectClient(std::vector<pollfd>::iterator pollfd);
		
		// DEBUG
		void printNickname();
		std::string t(const std::string& input);
		
};

#endif