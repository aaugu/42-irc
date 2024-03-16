/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:58 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/15 17:42:34 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <unistd.h>

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/messages.hpp"


/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Channel::Channel(std::string name, Client* client) : _name(name), _operatorConnected(true) {
	_clients.push_back(client);
}

Channel::~Channel(void) {}

/* ************************************************************************** */
/*                          PUBLIC MEMBER FUNCTIONS                           */
/* ************************************************************************** */

void	Channel::addClient(Client* client)
{
	if (client)
		_clients.push_back(client);
}

void	Channel::removeClient(Client* client) { (void) client; }

void	Channel::informClientArrival(Client* client)
{
	char	hostname[1024];
	gethostname(hostname, 1024);
	std::string	msg = client->getNickname() + " [~" + client->getNickname() + "@" + hostname + "] has joined " + _name;
	
	std::vector<Client*>::iterator	it = _clients.begin();
	for( ; it < _clients.end(); it++ )
	{
		sendMessage(msg, client->getFd());
	}
}

void	Channel::welcomeMessage(Client* client)
{
	std::string usersChannel = "[Users +" + _name + "]";
	sendMessage(usersChannel, client->getFd());

	std::vector<Client*>::iterator	it = _clients.begin();
	if ( _operatorConnected )
	{
		std::string	operatorName = "[@" + (*it)->getNickname() + "] ";
		sendMessage(operatorName, (*it)->getFd());
		it++;
	}
	
	for( ; it < _clients.end(); it++)
	{
		std::string	user = "[ " + (*it)->getNickname() + "] ";
		sendMessage(user, (*it)->getFd());
	}
}

/* ************************************************************************** */
/*                                   UTILS                                    */
/* ************************************************************************** */

/*
void	Channel::printUsers(void)
{
	std::string usersChannel = "[Users +" + _name + "]\n";
	sendMessage(usersChannel, client->getFd());

	std::vector<Client*>::iterator	it = _clients.begin();
	if ( _operatorConnected )
	{
		std::string	operatorName = "[@" + (*it)->getNickname() + "] ";
		sendMessage(operatorName, (*it)->getFd());
		it++;
	}
	
	for( ; it < _clients.end(); it++)
	{
		std::string	user = "[ " + (*it)->getNickname() + "] ";
		sendMessage(user, (*it)->getFd());
	}
	
	sendMessage("\n", client->getFd());
}

void	Channel::printStatus(void)
{
	
}

*/

/* ************************************************************************** */
/*                                 ACCESSORS                                  */
/* ************************************************************************** */

std::string	Channel::getName(void) {
	return ( _name );
}

bool	Channel::getOperatorConnected(void) {
	return ( _operatorConnected );
}

std::vector<Client*>	Channel::getClients(void) {
	return ( _clients );
}