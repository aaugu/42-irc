/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:29:27 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/26 13:36:51 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>

#include "../includes/CommandExec.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

void	CommandExec::topic(void)
{
	try
	{
		if (minNbParams((int)_msg->_paramsSplit.size(), 1) == false)
			throw ( ERR_NEEDMOREPARAMS(_client->getAddress(), _client->getNickname(), "topic") );

		std::string	channelName = _msg->_paramsSplit[0];
		if (_server->channelExists(channelName) == false)
			throw ( ERR_NOSUCHCHANNEL(_client->getAddress(), _client->getNickname(), channelName));

		std::vector<Channel>::iterator	channel = _server->getChannelByName(channelName);
		if ( channel->isUserPresent(_client) == false)
			throw ( ERR_NOTONCHANNEL(_client->getAddress(), _client->getNickname(), channel->getName()) );

		if ( _msg->_paramsSplit.size() == 1)
		{


		}
		else
		{
			if ( channel->getModeT() == false )
				throw ( ":Could no\r\n" );
			/* code */
		}


	}
	catch(const std::string& message)
	{
		_client->sendMessage(message);
	}
}


// void	CommandExec::sendTopic(Channel& channel)
// {
// 	if ( channel.getTopic() == "")
// 		_client->sendMessage( /* RPL_NOTOPIC*/ )
// 	else
// 		_client->sendMessage( /* RPL_TOPIC */)
// }

// void	CommandExec::setTopic(Channel& channel)
// {

// }
