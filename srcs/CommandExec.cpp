/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExec.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:53:34 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/15 17:35:56 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

#define ERR_NB_ARGS(command, min, max) ("Command: " + command + ": Not enough arguments, should be between" + min + " and " + max)

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

CommandExec::CommandExec(Server* server, Client* client, s_message* msg) : _server(server), _client(client), _msg(msg) {}

CommandExec::~CommandExec(void) {}

/* ************************************************************************** */
/*                                    UTILS                                   */
/* ************************************************************************** */

bool	CommandExec::invalidNbParams(int minNbParams, int maxNbParams)
{
	if ( (int)_msg->_paramsSplit.size() < minNbParams || (int)_msg->_paramsSplit.size() < maxNbParams)
		return (true);
	return (false);
}

/* ************************************************************************** */
/*                                    JOIN                                    */
/* ************************************************************************** */

void	CommandExec::join(void)
{
	std::cout << "Trying to join or create channel....\n";
	if (invalidNbParams(1, 2))
		return (sendMessage("join nb arg", _client->getFd()));

	checkChannelName(_msg->_paramsSplit[0]);

	std::vector<Channel>::iterator	channel = _server->getChannelByName(_msg->_paramsSplit[0]);
	if ( channel == _server->getChannels().end())
	{
		createChannel(_msg->_paramsSplit[0]);
		channel = _server->getChannelByName(_msg->_paramsSplit[0]);
	}
	else
		channel->addClient(_client);

	channel->informClientArrival(_client);
	_client->setCurrentChannel(&(*channel));
	channel->welcomeMessage(_client);
}

void	CommandExec::checkChannelName(std::string name)
{
	if ( name[0] != '#')
		name = "#" + name;
}

void	CommandExec::createChannel(std::string name)
{
	Channel	channel(name, _client);

	_server->addChannel(channel);
}
