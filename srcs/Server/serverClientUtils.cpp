
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverClientUtils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:33:19 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/26 10:45:08 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <arpa/inet.h>

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/messages.hpp"
#include "../includes/CommandExec.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

#define MAX_CONNECTIONS		"Server cannot accept more client \r\n"
#define SERVER_FULL			"Client attemped to connect but server is full"
#define ERR_CLIENT_NONEX	"Could not find client with this fd"

/* ************************************************************************** */
/*                          PRIVATE MEMBER FUNCTIONS                          */
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
	sendMessageTo(MAX_CONNECTIONS, sockfdClient);

	if (close(sockfdClient) < 0)
		throw std::runtime_error(errMessage(ERR_CLOSE, sockfdClient, strerror(errno)));

	printErrMessage(errMessage("Client", -1, SERVER_FULL));
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

	_clients.push_back(new Client(sockfdClient, address));
}

void	Server::addClientToListenPoll(int sockfdClient)
{
	pollfd	client;

	client.fd = sockfdClient;
	client.events = POLLIN | POLLOUT;

	_pollFds.push_back(client);
}

std::vector<Client*>::iterator	Server::getClientByFd(int sockfdClient)
{
	std::vector<Client*>::iterator it;
	for ( it = _clients.begin(); it < _clients.end(); it++ )
	{
		if ( (*it)->getFd() == sockfdClient)
			return (it);
	}
	return (it);
}

/* ************************************************************************** */
/*                          PUBLIC MEMBER FUNCTIONS                           */
/* ************************************************************************** */

void	Server::disconnectClient(Client *client)
{
	removeClientFromChannels(client);

	std::cout << "Client " << client->getFd() << ": disconnected" << std::endl;

	std::vector<pollfd>::iterator itP = getPollFdByFd(client->getFd());
	if ( itP == _pollFds.end() )
		return (printErrMessage(errMessage("Pollfd", client->getFd(), ERR_CLIENT_NONEX)));

	if (close(itP->fd) < 0)
		throw std::runtime_error(errMessage(ERR_CLOSE, itP->fd, strerror(errno)));
	_pollFds.erase(itP);

	_nbConnections--;

	if (clientExists(client->getNickname()))
	{
		std::vector<Client*>::iterator itC = getClientByFd(client->getFd());
		delete *itC;
		_clients.erase(itC);
	}
	else
		printErrMessage(errMessage("client", client->getFd(), ERR_CLIENT_NONEX));
}

bool Server::clientExists(std::string nickname){
	std::vector<Client*>::iterator it;
    for ( it = _clients.begin(); it < _clients.end(); it++ )
    {
        if ( (*it)->getNickname() == nickname)
            return(true);
    }
    return (false);
}

std::vector<Client*>::iterator	Server::getClientByNickname(std::string nickname)
{
    std::vector<Client*>::iterator it;
    for ( it = _clients.begin(); it < _clients.end(); it++ )
    {
        if ( (*it)->getNickname() == nickname)
            return(it);
    }
    return (it);
}

void	Server::removeClientFromChannels(Client *user)
{
	std::vector<Channel*>::iterator	channel = _channels.begin();
	for ( ; channel < _channels.end(); channel++ )
	{
		if ( (*channel)->isUserPresent(user) )
		{
			s_message	message;
			message._paramsSplit.clear();
			message._paramsSplit.push_back((*channel)->getName());
			message._paramsSplit.push_back(":disconnected");

			CommandExec	exec(this, user, &message);
			exec.part();
		}
	}
}