/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverClientUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:33:19 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/17 19:20:48 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/messages.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

#define MAX_CONNECTIONS		"Server cannot accept more client"
#define SERVER_FULL			"Client attemped to connect but server is full"
#define ERR_CLIENT_NONEX	"Could not find client with this fd"

/* ************************************************************************** */
/*                                CHANNEL UTILS                               */
/* ************************************************************************** */

int		Server::acceptNewClient(void)
{
	int					sockfdClient;
	struct sockaddr_in	addrClient;
	socklen_t			addrLenClient = sizeof(addrClient);

	sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, (socklen_t *)&addrLenClient);

	if (sockfdClient < 0)
		throw std::runtime_error(errMessage("Client : ", sockfdClient, strerror(errno)));
	return (sockfdClient);
}

void	Server::refuseClient(int sockfdClient)
{
	sendMessage(MAX_CONNECTIONS, sockfdClient);

	if (close(sockfdClient) < 0)
		throw std::runtime_error(errMessage(ERR_CLOSE, sockfdClient, strerror(errno)));

	printErrMessage(errMessage("Client", -1, SERVER_FULL));
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
	client.events = POLLIN | POLLOUT;

	_pollFds.push_back(client);
	_nbConnections++;
}

void	Server::createClient(int sockfdClient)
{
	struct sockaddr_in	clientAddress;
	socklen_t			clientAddressSize = sizeof(clientAddress);
	std::string			address;

	if (getsockname(sockfdClient, (struct sockaddr *)&clientAddress, &clientAddressSize) < 0)
	{
		printErrMessage(errMessage("Cannot get socket information of client", sockfdClient, strerror(errno)));
		address = "127.0.0.1";
		std::cout << "Localhost address set as default: " << address << std::endl;
	}
	else
		address = inet_ntoa(clientAddress.sin_addr);

	_clients.push_back(Client(sockfdClient, address));
}

void	Server::disconnectClient(std::vector<pollfd>::iterator pollfd)
{
	std::cout << "Client " << pollfd->fd << ": disconnected" << std::endl;
	if (close(pollfd->fd) < 0)
		throw std::runtime_error(errMessage(ERR_CLOSE, pollfd->fd, strerror(errno)));

	_nbConnections--;

	std::vector<Client>::iterator itC = getClientByFd(pollfd->fd);
	if ( itC == _clients.end() )
		return (printErrMessage(errMessage("Client", pollfd->fd, ERR_CLIENT_NONEX)));

	_pollFds.erase(pollfd);
	_clients.erase(itC);
}
