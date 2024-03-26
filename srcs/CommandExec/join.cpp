/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 16:21:16 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/26 11:35:59 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

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
#define ERR_INVITEONLYCHAN(address, client, channel) (":" + address + " 473 " + client + " " + channel + ":Cannot join channel (+k)\r\n")
#define ERR_BADCHANNELKEY(address, client, channel)  (":" + address + " 475 " + client + " " + channel + ":Cannot join channel (+i)\r\n")
#define ERR_CHANNELISFULL(address, client, channel)  (":" + address + " 471 " + client + " " + channel + ":Cannot join channel (+l)\r\n")

/* ************************************************************************** */
/*                                   JOIN                                     */
/* ************************************************************************** */

void	CommandExec::join(void)
{
	if (minNbParams((int)_msg->_paramsSplit.size(), 1) == false)
		return (_client->sendMessage("join nb arg \r\n"));

	checkChannelName(_msg->_paramsSplit[0]);

	std::vector<Channel>::iterator	channel = _server->getChannelByName(_msg->_paramsSplit[0]);
	if ( channel == _server->getChannels().end())
		return (createChannel(_msg->_paramsSplit[0]));

	if (channel->getModeI() == true)
		_client->sendMessage(ERR_INVITEONLYCHAN(_client->getAddress(), _client->getNickname(), channel->getName()));
	else if (channel->getModeL() == true && (int)channel->getUsers().size() >= channel->getUserLimit())
		_client->sendMessage(ERR_CHANNELISFULL(_client->getAddress(), _client->getNickname(), channel->getName()));
	else if (channel->getPassword() != "")
	{
		if (_msg->_paramsSplit.size() != 2 || channel->isPasswordValid(_msg->_paramsSplit[1]) == false)
			_client->sendMessage(ERR_BADCHANNELKEY(_client->getAddress(), _client->getNickname(), channel->getName()));
	}
	else
		joinChannel(*channel);
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */

void	CommandExec::checkChannelName(std::string& name)
{
	// check des caractères valides à ajouter

	if ( name[0] != '#')
		name = "#" + name;
}

void	CommandExec::createChannel(std::string name)
{
	Channel	channel(name, _client);

	_server->addChannel(channel);
	_client->setCurrentChannel(&channel);
	
	_client->sendMessage(RPL_JOIN(_client, channel.getName()));
	_client->sendMessage(RPL_NAMREPLY(_client->getAddress(), _client->getNickname(), channel.getName(), "@" + _client->getNickname()));
	_client->sendMessage(RPL_ENDOFNAMES(_client->getAddress(), _client->getNickname(), channel.getName()));
}

void	CommandExec::joinChannel(Channel& channel)
{
	channel.addUser(_client, false);
	_client->setCurrentChannel(&channel);

	channel.sendMessageToUsers(RPL_JOIN(_client, channel.getName()));

	std::string channelUserList = channel.getAllUsersList();
	_client->sendMessage(RPL_NAMREPLY(_client->getAddress(), _client->getNickname(), channel.getName(), channelUserList));
	_client->sendMessage(RPL_ENDOFNAMES(_client->getAddress(), _client->getNickname(), channel.getName()));
}