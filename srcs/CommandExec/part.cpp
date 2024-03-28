/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:19:52 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/28 15:44:21 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "../includes/CommandExec.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

#define RPL_PART(client, channel, message) (":" + USER(client) + " PART " + channel + " " + message + "\r\n")
#define ERR_NOTONCHANNEL(address, client, channel) (":" + address + " 442 " + client + " " + channel + ":You're not on that channel\r\n")

void	CommandExec::part(void)
{
	if (minNbParams((int)_msg->_paramsSplit.size(), 1) == false)
		return ( _client->sendMessage(ERR_NEEDMOREPARAMS(_client->getAddress(), _client->getNickname(), "part")));

	std::string	channelName = _msg->_paramsSplit[0];
	if (_server->channelExists(channelName))
	{
		std::vector<Channel>::iterator	channel = _server->getChannelByName(channelName);
		if ( channel->isUserPresent(_client) )
		{
			std::string	message = "";
			if ( _msg->_paramsSplit.size() > 1 )
				message += getFullMessage();
			channel->sendMessageToUsers(RPL_PART(_client, channel->getName(), message));
			if (channel->removeUser(_client) == 0)
			{
				std::cout << _client->getNickname() << " successfully removed from " << channel->getName() << std::endl;
				_server->closeChannel(channel->getName());
			}
		}
		else
			_client->sendMessage(ERR_NOTONCHANNEL(_client->getAddress(), _client->getNickname(), channel->getName()));
	}
	else
		_client->sendMessage(ERR_NOSUCHCHANNEL(_client->getAddress(), _client->getNickname(), channelName));
}
