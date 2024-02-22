/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/22 14:16:18 by aaugu            ###   ########.fr       */
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
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/error_handling.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Server::Server(void) {}

Server::Server(int port)
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

	std::cout << "Server successfully initialized!" << std::endl;
}

Server::~Server(void)
{
	// std::for_each (_clients.begin(), _clients.end(), closeClient);
	std::for_each (_clients.begin(), _clients.end(), deleteClient);
	close(_sockfd);
	std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::start(void) {

	std::cout << _sockfd << std::endl;

	int					ls;
	int					sockfdClient;
	struct sockaddr_in	addrClient;
	socklen_t			addrLenClient;

	while (true)
	{
		ls = listen(_sockfd, 5);
		if (ls == -1)
			throw std::runtime_error(errMessage("Server : ", strerror(errno)));
		// std::cout << "listen !" << std::endl;

		addrLenClient = sizeof(addrClient);
		sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, &addrLenClient);

		if (sockfdClient < 0)
			throw std::runtime_error(errMessage("Client : ", strerror(errno)));
		else
		{
			_clients.push_back(new Client(sockfdClient, addrClient, addrLenClient));
			std::cout << "Client " << sockfdClient << " connected." << std::endl;
			send((*_clients.back()).getSocket(), "hello\n", 5, 0);
		}
	}
}

void Server::stop(void) {
	this->run = 0;

	// std::for_each (_clients.begin(), _clients.end(), closeClient);
	
}

/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */


/* ************************************************************************** */
/*                            NON MEMBER FUNCTIONS                            */
/* ************************************************************************** */

// ------------------------------- Stop utils ------------------------------- //
// void    closeClient(Client* client) {
// 	int	sockfd = client->getSocket();

// 	if (sockfd > 0)
// 		close(sockfd);
// }

// ---------------------------- Destructor utils ---------------------------- //
void    deleteClient(Client* client) {
	int	sockfd = client->getSocket();

	if (client)
	{
		if (sockfd > 0)
			close(sockfd);
		delete client;
	}
}