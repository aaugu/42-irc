/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/27 15:24:49 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <list>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <err.h>
#include <cstring>
#include <unistd.h>
#include <algorithm>
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
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
		throw std::runtime_error(errMessage("Server : ", strerror(errno)));

	// Allow socket descriptor to be reuseable
	int on = 1;
	if ( setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
		throw std::runtime_error(errMessage("Server : ", strerror(errno)));

	// Set socket to be nonblocking. All of the sockets for the incoming connections 
	// will also be nonblocking since they will inherit that state from the listening socket
	int flags = fcntl( _sockfd, F_GETFL, 0 );
	if ( fcntl(_sockfd, F_SETFL, flags, O_NONBLOCK) == -1 )
		throw std::runtime_error(errMessage("Server : ", strerror(errno)));

	// Bind the socket
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw std::runtime_error(errMessage("Server : ", strerror(errno)));

	// Allocate memory for server nb max of connections fds
	_pollFds = new std::vector<pollfd>(SOMAXCONN + 1);

	std::cout << "Server successfully initialized!" << std::endl;
}

Server::~Server(void)
{
	closePollFds();

	delete [] _pollFds;
	std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::start(void) {

	// Set the listen back log
	int ls = listen(_sockfd, SOMAXCONN);
	if (ls == -1)
		throw std::runtime_error(errMessage("Server : ", strerror(errno)));

	std::cout << _sockfd << std::endl;

	std::vector<pollfd>	&fds = *_pollFds;
	fds.begin()->fd = _sockfd;
	fds.begin()->events = POLLIN;

	int					sockfdClient;
	struct sockaddr_in	addrClient;
	socklen_t			addrLenClient;

	while (true) // signal arret du serveur
	{
		int i = 0;
		// std::cout << "listen !" << std::endl;

		waitForEvent();

		addrLenClient = sizeof(addrClient);
		sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, &addrLenClient);

		if (sockfdClient == -1)
			throw std::runtime_error(errMessage("Client : ", strerror(errno)));
		else
		{
			std::vector<pollfd>::iterator it;
			for (it = fds.begin(); i < nbConnections + 1; it++, i++) { continue; }
			(*it).fd = sockfdClient;
			(*it).events = POLLIN;
			std::cout << "Client " << sockfdClient << " connected." << std::endl;
			send((*it).fd, "Welcome\n", 8, 0);
		}
		// char buf[4096];
		// memset( buf, 0, sizeof( buf ) );
		// for ( int i = 1; i <= nbConnections; i++ ){
		// 	if ( fds[i].fd != -1 && fds[i].revents & POLLIN ){
		// 		if (recv(fds[i].fd, buf, sizeof( buf ), 0 ) == -1){
		// 			throw std::runtime_error(errMessage("Server : ", strerror(errno)));
		// 		}
		// 	}
		// }

	}
}

void Server::stop(void) {
	this->run = 0;

	// std::for_each (_clients.begin(), _clients.end(), closeClient);
	
}


/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

void	Server::waitForEvent(void)
{
	int	timeout = 0;
	std::vector<pollfd>	&fds = *_pollFds;

	if ( poll(fds.data(), nbConnections + 1, timeout) == -1 )
		throw std::runtime_error(errMessage("Server : ", strerror(errno)));
}


/* ************************************************************************** */
/*                            NON MEMBER FUNCTIONS                            */
/* ************************************************************************** */

// ---------------------------- Destructor utils ---------------------------- //
void    Server::closePollFds(void) {

	std::vector<pollfd>				&fds = *_pollFds;
	std::vector<pollfd>::iterator	it;
	int	sockfd;
	int i = 0;
	
	for (it = fds.begin(); i < nbConnections + 1; it++, i++)
	{
		sockfd = (*it).fd;
		if (sockfd > 0)
			close(sockfd);
	}
}