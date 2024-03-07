/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/07 14:32:43 by aaugu            ###   ########.fr       */
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
		throw std::runtime_error(errMessage("Server1", -1, strerror(errno)));

	// Allow socket descriptor to be reuseable
	int optionValue = 1;
	if ( setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEPORT, &optionValue, sizeof(optionValue)) == -1) {
		close(_sockfd);
		throw std::runtime_error(errMessage("Server2", -1, strerror(errno)));
	}

	// Set socket to be nonblocking. All of the sockets for the incoming connections
	// will also be nonblocking since they will inherit that state from the listening socket
	if ( fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1 ) {
		close(_sockfd);
		throw std::runtime_error(errMessage("Server3", -1, strerror(errno)));
	}

	// Bind the socket
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) == -1) {
		close(_sockfd);
		throw std::runtime_error(errMessage("Server4", -1, strerror(errno)));
	}

	// Signals
	signal( SIGINT, sig::signalHandler );

	std::cout << "Server successfully initialized! Listening for connections on port " << port << std::endl;
}

Server::~Server(void) {
	closePollFds();
	std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::run(void)
{
	setListenBackLog();

	while (sig::stopServer == false)
	{
		waitForEvent();

		// if ( _pollFds.front().revents & POLLIN )
		// 	createClientConnection();

		std::vector<pollfd>::iterator itP = _pollFds.begin();
		for ( ; itP != _pollFds.end(); itP++ )
		{
			if ( itP->revents & POLLIN ) // if the data is available to read on the fd/socket
			{
				if ( itP->fd == _sockfd )	// server side
				{
					createClientConnection();
				}
				else						// client side
					handleClientInput(itP);
			}
			else if ( itP->revents & POLLOUT ) // if the data is available to write on the fd/socket
			{
				// TO DO

			}
			else if ( itP->revents & POLLERR ) // if an error occurred on the fd/socket
			{
				// TO DO

			}
		}
		


	}
}

void Server::stop(void)
{
	// this->run = 0;
	closePollFds();
	std::cerr << "!!!! Serveur STOP !!!!" << std::endl;
}


/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

// ---------------------- Main function sub functions ----------------------- //

void	Server::setListenBackLog(void)
{
	if ( listen(_sockfd, SOMAXCONN) == -1 )
		throw std::runtime_error(errMessage("Server5", -1, strerror(errno)));

	pollfd	listenFd;
	listenFd.fd = _sockfd;
	listenFd.events = POLLIN;

	_pollFds.push_back(listenFd);
}

void	Server::waitForEvent(void)
{
	int	timeout = -1;

	if ( poll(&_pollFds[0], (nfds_t) _nbConnections + 1, timeout) == -1 && sig::stopServer == false)
		throw std::runtime_error(errMessage("Server6", -1, strerror(errno)));	
}

void	Server::createClientConnection(void)
{
	int	sockfdClient;

	sockfdClient = acceptNewClient();
	std::cout << sockfdClient << std::endl;
	if (sockfdClient == -1)
		return (printErrMessage(errMessage("Client", sockfdClient, strerror(errno))));

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

void	Server::handleClientInput(std::vector<pollfd>::iterator clientPollFd)
{
	char	buffer[4096];
	size_t readBytes = recv(clientPollFd->fd, buffer, 1024, 0);

	// infos irssi à récup

	if ( (int)readBytes == -1 )
		throw std::runtime_error(errMessage("Server7", clientPollFd->fd, strerror(errno)));
	else if (readBytes == 0)
	{
		
		return (disconnectClient(clientPollFd));
	}
	else
	{
		buffer[readBytes] = '\0';
		 // checkCapFlags(buffer, (*itP).fd);
		// *sockfdClient = itP->fd;
		std::cout << "Client " << clientPollFd->fd << ": " << buffer;
		return ;
	}
}

// void	Server::manageClientInput(std::string input, int sockfdClient)
// {	
// 	// get sender details
// 	// parse command and parameters
// 	// check validity of command
// 	// check context
// 	// check if client is authorized to perform
// 	// perform command
// 	std::cout << "Client " << sockfdClient << ": " << input;
// }

// ------------------------------ Client Utils ------------------------------ //

int		Server::acceptNewClient(void)
{
	int					sockfdClient;
	struct sockaddr_in	addrClient;
	socklen_t			addrLenClient = sizeof(addrClient);

	sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, &addrLenClient);
	std::cout << sockfdClient << std::endl; 

	return (sockfdClient);
}

void	Server::addClientToListenPoll(int sockfdClient)
{
	if (_nbConnections >= SOMAXCONN)
		return (printErrMessage(errMessage("Server8", -1, "cannot accept more client")));

	pollfd	client;

	client.fd = sockfdClient;
	client.events = POLLIN | POLLOUT;

	_pollFds.push_back(client);
	_nbConnections++;
}

void	Server::disconnectClient(std::vector<pollfd>::iterator pollfd)
{
	if (close(pollfd->fd) == -1)
		throw std::runtime_error(errMessage("Client", (*pollfd).fd, strerror(errno)));
	std::cout << "Client " << (*pollfd).fd << ": disconnected" << std::endl;

	_nbConnections--;

	std::vector<Client>::iterator itC;
	if ( pollfd->fd != _sockfd )
	{
		itC = getClientByFd(pollfd->fd);
		if ( itC == _clients.end() )
			printErrMessage(errMessage("Client", pollfd->fd, "could not find client with this fd"));
	}
	_pollFds.erase(pollfd);
	_clients.erase(itC);
	std::cout << "ici\n";
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
				throw std::runtime_error(errMessage("Client", sockfd, strerror(errno)));
		}
	}
}

/* ************************************************************************** */
/*                                  ACCESSORS                                 */
/* ************************************************************************** */

std::vector<Client>::iterator	Server::getClientByFd(int sockfdClient)
{
	std::vector<Client>::iterator it;
	for ( it = _clients.begin(); it != _clients.end(); it++ )
	{
		if ( it->getFd() == sockfdClient)
			return (it);
	}
	return (it);
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