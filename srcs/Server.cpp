/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/29 11:16:21 by lvogt            ###   ########.fr       */
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
#include "Server.hpp"
#include "Error_handling.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Server::Server(void) {}

Server::Server(int port) : _nbConnections(0)
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

	while (true)
	{
		int i = 0;

		waitForEvent();
		if (fds[i].revents & POLLIN)
        {
			addrLenClient = sizeof(addrClient);
			sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, &addrLenClient);

			if (sockfdClient == -1)
				throw std::runtime_error(errMessage("Client : ", strerror(errno)));
			else
			{
				std::vector<pollfd>::iterator it;
				for (it = fds.begin(); i < _nbConnections + 1; it++, i++) { continue; }
				(*it).fd = sockfdClient;
				(*it).events = POLLIN;
				_nbConnections++;
				std::cout << "Client " << sockfdClient << " connected." << std::endl;
				send((*it).fd, "Welcome\n", 8, 0);
			}
		}

		char buffer[4096];
		memset( buffer, 0, sizeof( buffer ) );

		for (int i = 1; i < _nbConnections + 1; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytesRead > 0)
                {
                    buffer[bytesRead] = '\0';
                    if (std::strncmp(buffer, "CAP LS", 5) == 0) {
                        std::cout << "gestion de CAP LS" << std::endl;
                        const char* response = "CAP * LS :\n";
                        send(fds[i].fd, response, strlen(response), 0);
                    }
                    std::cout << "Received from client " << fds[i].fd << ": '" << buffer << "'" << std::endl;
                }
                else if (bytesRead == 0)
                {
                    std::cout << "Client " << fds[i].fd << " disconnected." << std::endl;
                    close(fds[i].fd);
                    for (int j = i; j < _nbConnections; ++j)
                        _pollFds[j] = _pollFds[j + 1];
                    --_nbConnections;
					std::cout << "debug nb client : " << _nbConnections << std::endl;
                }
                else
                {
                    std::cerr << "Error receiving data from client " << fds[i].fd << std::endl;
                }
			}
		}

	}
}

/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

void	Server::waitForEvent(void)
{
	int	timeout = 0;
	std::vector<pollfd>	&fds = *_pollFds;

	if ( poll(fds.data(), _nbConnections + 1, timeout) == -1 )
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
	
	for (it = fds.begin(); i < ___WCTYPE_H_ + 1; it++, i++)
	{
		sockfd = (*it).fd;
		if (sockfd > 0)
			close(sockfd);
	}
}