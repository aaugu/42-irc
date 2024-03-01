/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/01 10:22:48 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include <poll.h>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/error_handling.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Server::Server(void) {}

Server::Server(int port) : nbConnections(0)
{
	std::cout << "Initializing server..." << std::endl;

	// Create an AF_INET6 stream socket to receive incoming connections on
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	// Allow socket descriptor to be reuseable
	int on = 1;
	if ( setsockopt(sockfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	// Set socket to be nonblocking. All of the sockets for the incoming connections
	// will also be nonblocking since they will inherit that state from the listening socket
	// int flags = fcntl( sockfd, F_GETFL, 0 );
	// if ( fcntl(sockfd, F_SETFL, flags, O_NONBLOCK) == -1 )
	// 	throw std::runtime_error(errMessage("Server3.1 : ", -1, strerror(errno)));

	// Bind the socket
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	std::cout << "Server successfully initialized!" << std::endl;
}

Server::~Server(void)
{
	closePollFds();

	std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::start(void) {

	setListenBackLog();

	while (true) // signal arret du serveur
	{
		waitForEvent();
		if (pollFds.front().revents & POLLIN)
			acceptNewClient();

		int			sockfdClient = -1;
		std::string	clientInput = "";
		getClientInput(clientInput, &sockfdClient);
		// if (clientInput == "q\n")
		// {
		// 	std::cout << "Exiting program..." << std::endl;
		// 	return ;
		// }
		if (sockfdClient != -1 && clientInput.empty() == false)
			executeClientInput(clientInput, sockfdClient);
	}
}

void Server::stop(void) {
	this->run = 0;
}


/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

// ---------------------- Main function sub functions ----------------------- //
void	Server::setListenBackLog(void)
{
	// Set the listen back log
	int ls = listen(sockfd, SOMAXCONN);
	if (ls == -1)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));

	pollfd	listenFd;
	listenFd.fd = sockfd;
	listenFd.events = POLLIN;

	pollFds.push_back(listenFd);
}

void	Server::waitForEvent(void)
{
	int	timeout = 0;

	if ( poll(&pollFds[0], (nfds_t) nbConnections + 1, timeout) == -1 )
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));
}

void	Server::acceptNewClient(void)
{
	int					sockfdClient;
	struct sockaddr_in	addrClient;
	socklen_t			addrLenClient = sizeof(addrClient);

	sockfdClient = accept(sockfd, (struct sockaddr *)&addrClient, &addrLenClient);

	if (sockfdClient == -1)
		throw std::runtime_error(errMessage("Client : ", sockfdClient, strerror(errno)));
	else
	{
		if (nbConnections <= SOMAXCONN)
			addClientToListenPoll(sockfdClient);
		else
			std::runtime_error(errMessage("Server : ", -1, "cannot accept more client"));
	}
}



void	Server::getClientInput(std::string& clientInput, int* sockfdClient)
{
	char buffer[1024] = {0};

	std::vector<pollfd>::iterator it;
	for (it = pollFds.begin() + 1; it != pollFds.end(); it++)
	{
		if ((*it).revents & POLLIN)
		{
			size_t readBytes = recv((*it).fd, buffer, 1024, 0);
			buffer[readBytes] = '\0';

			if ( (int)readBytes == -1 )
				throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));
			else if (readBytes == 0)
				return (disconnectClient(it));
			else
			{
				clientInput = static_cast<std::string>(buffer);
				*sockfdClient = (*it).fd;
				return ;
			}
		}
	}
}

void	Server::executeClientInput(std::string clientInput, int sockfdClient)
{
	std::cout << "Client " << sockfdClient << ": " << clientInput;
}

// ---------------------- Utils ----------------------- //

void	Server::addClientToListenPoll(int sockfdClient)
{
	pollfd	newClient;

	newClient.fd = sockfdClient;
	newClient.events = POLLIN;

	pollFds.push_back(newClient);
	nbConnections++;

	std::cout	<< "New connection : "
				<< "[SOCKET_FD : "	<< sockfdClient
				<< " , IP : "		<< inet_ntoa(addr.sin_addr)
				<< " , PORT : "		<< ntohs(addr.sin_port) << "]"
				<< std::endl;

	send(sockfdClient, "Welcome\n", 8, 0);
}

void	Server::disconnectClient(std::vector<pollfd>::iterator client)
{
	std::cout << "Client " << (*client).fd << ": disconnected" << std::endl;
	close((*client).fd);
	nbConnections--;
	pollFds.erase(client);
}

// ---------------------------- Destructor utils ---------------------------- //
void    Server::closePollFds(void)
{
	std::vector<pollfd>::iterator	it;
	int	sockfd;

	for (it = pollFds.begin(); it != pollFds.end(); it++)
	{
		sockfd = (*it).fd;
		if (sockfd > 0)
			close(sockfd);
	}
}
