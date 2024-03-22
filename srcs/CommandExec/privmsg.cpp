
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:32:28 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/20 18:43:12 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "../includes/CommandExec.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// RESPONSES														// should be username

												// shoud be username
// #define USER(user) (user->getNickname() + "!" + user->getNickname() + "@" + user->getAddress())
// #define RPL_JOIN(client, channel) (":" + USER(client) + " JOIN " + channel + "\r\n")
// #define RPL_ENDOFNAMES(address, client, channel) (":" + address + " 366 " + client + " " + channel + " :End of /NAMES list\r\n")
// #define RPL_NAMREPLY(address, client, channel, nickname) (":" + address + " 353 " + client + " = " + channel + " :" + nickname + "\r\n")

// ERRORS
	// Channel
#define ERR_CANNOTSENDTOCHAN(address, client, channel) (":" + address + " 404 " + client + " " + channel + ":You are not part of this channel\r\n")
	// Private messages

// #define ERR_CHANNELISFULL(address, client, channel)  (":" + address + " 471 " + client + " " + channel + ":Cannot join channel (+l)\r\n")

/* ************************************************************************** */
/*                                 PRIVMSG                                    */
/* ************************************************************************** */

void	CommandExec::privmsg(void)
{
	if (minNbParams((int)_msg->_paramsSplit.size(), 2) == false)
		return (_client->sendMessage("privmsg nb arg \r\n"));

	std::string	target = _msg->_paramsSplit[0];
	std::string	message = getFullMessage();

	if (target[0] == '#')
		sendMessageToChannel(target, message);
	else
		sendPrivateMessage(target, message);
}

void CommandExec::sendMessageToChannel(std::string target, std::string message)
{
	if (_server->channelExists(target))
	{
		std::vector<Channel>::iterator	channel = _server->getChannelByName(target);
		if ( channel->isUserPresent(_client) )
		{
			std::string	response = RPL_PRIVMSG(_client, channel->getName(), message);
			channel->sendMessageToUsersExceptSender(_client, response);
		}
		else
			_client->sendMessage(ERR_CANNOTSENDTOCHAN(_client->getAddress(), _client->getNickname(), channel->getName()));

	}
	else
		_client->sendMessage(ERR_NOSUCHCHANNEL(_client->getAddress(), _client->getNickname(), target));
}

void CommandExec::sendPrivateMessage(std::string target, std::string message)
{
	if (_server->clientExists(target))
	{
		std::vector<Client>::iterator	user = _server->getClientByNickname(target);
		std::string	response = RPL_PRIVMSG(_client, user->getNickname(), message);
		user->sendMessage(response);
	}
	else
		_client->sendMessage(ERR_NOSUCHNICK(_client->getAddress(), target));
}
