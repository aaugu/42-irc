/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:32:28 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/05 16:10:52 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "../includes/CommandExec.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

#define ERR_CANNOTSENDTOCHAN(address, client, channel) (":" + address + " 404 " + client + " " + channel + " :You are not part of this channel\r\n")

/* ************************************************************************** */
/*                                 PRIVMSG                                    */
/* ************************************************************************** */

void	CommandExec::privmsg(void)
{
	if (minNbParams((int)_msg->_paramsSplit.size(), 2) == false)
		return ( _client->sendMessage(ERR_NEEDMOREPARAMS(_client->getAddress(), _client->getNickname(), "privmsg")));

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
