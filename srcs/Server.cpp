/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/04 11:40:20 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <algorithm>

#include "../includes/Server.hpp"
#include "../includes/errorHandling.hpp"
#include "../includes/signal.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

// Server::Server(void) {}

Server::Server(int port) : _nbConnections(0)
{
	std::cout << "Initializing server..." << std::endl;

	// Create an AF_INET6 stream socket to receive incoming connections on
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	// Allow socket descriptor to be reuseable
	int on = 1;
	if ( setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	// Set socket to be nonblocking. All of the sockets for the incoming connections
	// will also be nonblocking since they will inherit that state from the listening socket
	if ( fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1 )
		throw std::runtime_error(errMessage("Server3.1 : ", -1, strerror(errno)));

	// Bind the socket
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	// Signals
	signal( SIGINT, sig::signalHandler );

	std::cout << "Server successfully initialized!" << std::endl;
}

Server::~Server(void) {
	std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::start(void)
{
	setListenBackLog();

	while (sig::stopServer == false)
	{
		waitForEvent();
		if (_pollFds.front().revents & POLLIN)
			addNewClient();
		
		int			sockfdClient = -1;
		std::string	clientInput = "";

		getClientInput(clientInput, &sockfdClient);

		if (sockfdClient != -1 && clientInput.empty() == false)
		{
			manageClientInput(clientInput, sockfdClient);
		}
	}
}

void Server::stop(void)
{
	this->run = 0;
	closePollFds();
	std::cerr << "!!!! Serveur STOP !!!!" << std::endl;
}


/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

// ---------------------- Main function sub functions ----------------------- //

void	Server::setListenBackLog(void)
{
	int ls = listen(_sockfd, SOMAXCONN);
	if (ls == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	pollfd	listenFd;
	listenFd.fd = _sockfd;
	listenFd.events = POLLIN;

	_pollFds.push_back(listenFd);
}

void	Server::waitForEvent(void)
{
	int	timeout = 0;

	if ( poll(&_pollFds[0], (nfds_t) _nbConnections + 1, timeout) == -1 && sig::stopServer == false)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));	
}

void	Server::addNewClient(void)
{
	int	sockfdClient;

	sockfdClient = acceptNewClient();
	addClientToListenPoll(sockfdClient);
	// probablement une fonction qui recup les infos
	_clients.push_back(Client(sockfdClient));

	std::cout	<< "New connection : "
				<< "[SOCKET_FD : "	<< sockfdClient
				<< " , IP : "		<< inet_ntoa(_addr.sin_addr)
				<< " , PORT : "		<< ntohs(_addr.sin_port) << "]"
				<< std::endl;
	
	send(sockfdClient, "Welcome\n", 8, 0);
}

void	Server::getClientInput(std::string& clientInput, int* sockfdClient)
{
	char buffer[4096] = {0};

	std::vector<pollfd>::iterator itP = _pollFds.begin() + 1;
	std::vector<Client>::iterator itC = _clients.begin();
	for ( ; itP != _pollFds.end() || itC != _clients.end(); itP++, itC++ )
	{
		if ((*itP).revents & POLLIN)
		{
			size_t readBytes = recv((*itP).fd, buffer, 1024, 0);

			// infos irssi à récup

			if ( (int)readBytes == -1 )
				throw std::runtime_error(errMessage("Server : ", (*itP).fd , strerror(errno)));
			else if (readBytes == 0)
				return (disconnectClient(itP, itC));
			else
			{
				buffer[readBytes] = '\0';
				clientInput = static_cast<std::string>(buffer); // checkCapFlags(buffer, (*itP).fd);
				*sockfdClient = (*itP).fd;
				return ;
			}
		}
	}
}

void	Server::manageClientInput(std::string input, int sockfdClient)
{	
	// get sender details
	// parse command and parameters
	// check validity of command
	// check context
	// check if client is authorized to perform
	// perform command
	std::cout << "Client " << sockfdClient << ": " << input;
}

// ------------------------------ Client Utils ------------------------------ //

int		Server::acceptNewClient(void)
{
	int					sockfdClient;
	struct sockaddr_in	addrClient;
	socklen_t			addrLenClient = sizeof(addrClient);

	sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, &addrLenClient);

	if (sockfdClient == -1)
		throw std::runtime_error(errMessage("Client : ", sockfdClient, strerror(errno)));

	return (sockfdClient);
}

void	Server::addClientToListenPoll(int sockfdClient)
{
	if (_nbConnections >= SOMAXCONN)
	{
		std::cerr << errMessage("Server : ", -1, "cannot accept more client") << std::endl;
		return ;
	}

	pollfd	client;

	client.fd = sockfdClient;
	client.events = POLLIN;

	_pollFds.push_back(client);
	_nbConnections++;
}

void	Server::disconnectClient(std::vector<pollfd>::iterator pollfd, std::vector<Client>::iterator client)
{
	if (close((*pollfd).fd) == -1)
		throw std::runtime_error(errMessage("Client : ", (*pollfd).fd, strerror(errno)));
	std::cout << "Client " << (*pollfd).fd << ": disconnected" << std::endl;

	_nbConnections--;
	_pollFds.erase(pollfd);
	_clients.erase(client);
}

// ------------------------------ Input Utils ------------------------------- //

// std::string	Server::checkCapFlags(char* buffer, int sockfdClient)
// {

// 	if ( std::strncmp(buffer, "CAP LS", 5) == 0 ) {
// 		const char* response = "CAP * LS :\n";
// 		send(sockfdClient, response, strlen(response), 0);
// 	}
// 	// else
// 	// 	return (t(static_cast<std::string>(buffer)));
		
// 	return ( static_cast<std::string>(buffer) );
// }

// ---------------------------- Stop signal utils --------------------------- //
void    Server::closePollFds(void)
{
	std::vector<pollfd>::iterator	it;
	int	sockfd;

	for (it = _pollFds.begin(); it != _pollFds.end(); it++)
	{
		sockfd = (*it).fd;
		if (sockfd > 0)
		{
			if (close(sockfd) == -1)
				throw std::runtime_error(errMessage("Client : ", sockfd, strerror(errno)));
		}
	}
}

// /* ************************************************************************** */
// /*                            NON MEMBER FUNCTIONS                            */
// /* ************************************************************************** */

// std::string t(const std::string& input) {
//     std::string result;
//     for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
//         char c = *it;
//         switch (c) {
//             case '\n':
//                 result += "\\n";
//                 break;
//             case '\r':
//                 result += "\\r";
//                 break;
//             case '\t':
//                 result += "\\t";
//                 break;
//             // Ajoutez d'autres caractères spéciaux si nécessaire
//             default:
//                 result += c;
//                 break;
//         }
//     }
//     return result;
// }