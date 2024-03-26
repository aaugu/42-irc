/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:10 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/26 11:31:50 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <netinet/in.h>
# include <vector>
# include <poll.h>
# include <string>
# include "../includes/Channel.hpp"

#define ERR_CLIENT_NONEX	"Could not find client with this fd"
#define ERR_CLOSE			"Could not close file descriptor"

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
        std::string             _opPass;
		std::vector<Channel>	_channels;

		// Start() sub functions
		void	startServer(void);
		void	waitForEvent(void);
		void	createClientConnection(void);
		void	getClientInput(std::vector<pollfd>::iterator clientPollFd, std::string& clientInput);
		void	parseClientInput(std::string clientInput, int sockfdClient);
		void	executeClientInput(Server &server, std::vector<pollfd>::iterator pollfd);

		// Client Utils
		int								acceptNewClient(void);
		void							refuseClient(int sockfdClient);
		void							createClient(int sockfdClient);
		void							addClientToListenPoll(int sockfdClient);
		std::vector<Client>::iterator	getClientByFd(int sockfdClient);

		// Input utils
		int		getLine(int fd, std::string &line);

		// Destructor utils
		void    closePollFds(void);

		// Accessors
		std::vector<pollfd>::iterator 	getPollFdByFd(int sockfd);

	public :
		Server(int port, std::string password);
		~Server(void);

		void run(void);

		// Accessors
		std::vector<std::string>		getNicknameList(void);
        std::string                     getOpPass(void);
		std::string						get_password(void) const;
		std::vector<Client>				getClients(void);
		std::vector<Channel>			getChannels(void);

		// Channel utils
		void							addChannel(Channel& channel);
		void							removeChannel(std::vector<Channel>::iterator channel);
		bool							channelExists(std::string name);
		std::vector<Channel>::iterator	getChannelByName(std::string name);
		void							closeChannel(std::string name);
		Channel *						getptrChannelByName(std::string name);

		// Client utils
		void							disconnectClient(Client *client);
		bool 							clientExists(std::string nickname);
        std::vector<Client>::iterator	getClientByNickname(std::string nickname);

		// DEBUG
		void printNickname(void);
		std::string t(const std::string& input);

};

#endif
