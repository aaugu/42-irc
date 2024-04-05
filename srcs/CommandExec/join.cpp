/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 16:21:16 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/05 16:09:52 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>

#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// RESPONSES
#define RPL_JOIN(client, channel) (":" + USER(client) + " JOIN " + channel + "\r\n")
#define RPL_ENDOFNAMES(address, client, channel) (":" + address + " 366 " + client + " " + channel + " :End of /NAMES list\r\n")
#define RPL_NAMREPLY(address, client, channel, nickname) (":" + address + " 353 " + client + " = " + channel + " :" + nickname + "\r\n")

// ERRORS
#define ERR_INVITEONLYCHAN(address, client, channel) (":" + address + " 473 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_BADCHANNELKEY(address, client, channel)  (":" + address + " 475 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_CHANNELISFULL(address, client, channel)  (":" + address + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")

/* ************************************************************************** */
/*                                   JOIN                                     */
/* ************************************************************************** */

void	CommandExec::join(void)
{
	if (minNbParams((int)_msg->_paramsSplit.size(), 1) == false)
		return ( _client->sendMessage(ERR_NEEDMOREPARAMS(_client->getAddress(), _client->getNickname(), "join")));

	std::vector<std::string>	channelNames = getChannelNames(_msg->_paramsSplit[0]);

	std::vector<std::string>::iterator	channelName;
	for (channelName = channelNames.begin(); channelName < channelNames.end(); channelName++)
	{
		if ( isChannelNameValid(*channelName) == false )
		{
			_client->sendMessage("Channel name should not contain ',' or ' ' or [^G / 7]\r\n");
			continue ;
		}

		if ( _server->channelExists(*channelName) == false )
		{
			createChannel(*channelName);
			continue ;
		}

		std::vector<Channel>::iterator	channel = _server->getChannelByName(*channelName);
		if ( channel->getModeI() == true && channel->isUserOnWhitelist(_client) == false ) {
			_client->sendMessage(ERR_INVITEONLYCHAN(_client->getAddress(), _client->getNickname(), channel->getName()));
		}
		else if ( channel->getModeL() == true && (int)channel->getUsers().size() >= channel->getUserLimit() )
			_client->sendMessage(ERR_CHANNELISFULL(_client->getAddress(), _client->getNickname(), channel->getName()));
		else if (channel->getModeK() == true)
		{
			if ( _msg->_paramsSplit.size() == 2 || channel->isPasswordValid(_msg->_paramsSplit[1]) == true )
				joinChannel(*channel);
			else
				_client->sendMessage(ERR_BADCHANNELKEY(_client->getAddress(), _client->getNickname(), channel->getName()));
		}
		else if ( channel->isUserPresent(_client) )
			_client->sendMessage("You already have joined channel " + channel->getName() + "\r\n");
		else
			joinChannel(*channel);
	}

}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */

std::vector<std::string>	CommandExec::getChannelNames(std::string names)
{
	std::vector<std::string>	channelNames;
 	std::stringstream			ss(names);
 
    while (ss.good())
	{
		std::string	channelName;
        getline(ss, channelName, ',');
        channelNames.push_back(channelName);
    }

	return (channelNames);
}


bool	CommandExec::isChannelNameValid(std::string& name)
{
	if ( name.find(7) != name.npos )
		return (false);
	return (true);
}

void	CommandExec::createChannel(std::string name)
{
	if (name[0] != '#')
		return ( _client->sendMessage("Channel name should begin with '#'\r\n") );
	
	Channel	channel(name, _client);

	_server->addChannel(channel);

	_client->sendMessage(RPL_JOIN(_client, channel.getName()));
	_client->sendMessage(RPL_NAMREPLY(_client->getAddress(), _client->getNickname(), channel.getName(), "@" + _client->getNickname()));
	_client->sendMessage(RPL_ENDOFNAMES(_client->getAddress(), _client->getNickname(), channel.getName()));

	std::cout	<< "Channel " << channel.getName() << " created with "
				<< _client->getNickname() << " as default operator\n";
}

void	CommandExec::joinChannel(Channel& channel)
{
	channel.addUser(_client, false);
	channel.sendMessageToUsers(RPL_JOIN(_client, channel.getName()));

	if (channel.getTopic() != "")
		_client->sendMessage(RPL_TOPIC(_client->getNickname(), channel.getName(), channel.getTopic()));

	std::string channelUserList = channel.getAllUsersList();
	_client->sendMessage(RPL_NAMREPLY(_client->getAddress(), _client->getNickname(), channel.getName(), channelUserList));
	_client->sendMessage(RPL_ENDOFNAMES(_client->getAddress(), _client->getNickname(), channel.getName()));

	if ( channel.isUserOnWhitelist(_client) )
		channel.removeUserFromWhitelist(_client);

	std::cout 	<< "Client " << _client->getFd()
				<< " with nickname " << _client->getNickname()
				<< " has joined channel " << channel.getName() << std::endl;
}
