/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 13:09:59 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/03 13:12:46 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// RESPONSE
#define RPL_INVITING(client, target, channel) (":" + USER(client) + " INVITE " + target + " " + channel + "\r\n")

// ERROR
#define ERR_USERONCHANNEL(address, nickname, channel) (":" + address + " 443 " + nickname + " " + channel + " :is already on channel\r\n")

/* ************************************************************************** */
/*                                   INVITE                                   */
/* ************************************************************************** */

void	CommandExec::invite(void)
{
	try {
		if (minNbParams((int)_msg->_paramsSplit.size(), 2) == false)
			throw ( ERR_NEEDMOREPARAMS(_client->getAddress(), _client->getNickname(), "invite") );

		std::string	nickname = _msg->_paramsSplit[0];
		if (_server->clientExists(nickname) == false)
			throw ( ERR_NOSUCHNICK(_client->getAddress(), _client->getNickname()) );

		std::string	channelName = _msg->_paramsSplit[1];
		if (_server->channelExists(channelName) == false)
			throw ( ERR_NOSUCHCHANNEL(_client->getAddress(), _client->getNickname(), channelName));

		Channel*	channel = _server->getptrChannelByName(channelName);
		Client*		target = &(*_server->getClientByNickname(nickname));
		if ( channel->isUserPresent(target))
			throw ( ERR_USERONCHANNEL(target->getAddress(), target->getNickname(), channel->getName()) );

		if ( channel->isOperator(_client) && channel->isUserPresent(_client))
			launchInvitation(target, channel);
		else
			throw ( ERR_CHANOPRIVSNEEDED(channel->getName()) );
	}
	catch(const std::string& message) {
		_client->sendMessage(message);
	}
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */

void	CommandExec::launchInvitation(Client* target, Channel* channel)
{
	if ( channel->getModeI() == true && channel->isUserOnWhitelist(target) == false)
		channel->addUserToWhitelist(target);
	
	target->sendMessage(RPL_INVITING(_client, target->getNickname(), channel->getName()));
}
