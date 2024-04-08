/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:29:27 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/08 10:04:17 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandExec.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

#define RPL_SET_TOPIC(client, channel, topic) (":" + USER(client) + " TOPIC " + channel + " " + topic + "\r\n")
#define RPL_NOTOPIC(address, client, channel) (":" + address + " 331 " + client + " " + channel + " :No topic is set\r\n")

/* ************************************************************************** */
/*                                   TOPIC                                    */
/* ************************************************************************** */

void	CommandExec::topic(void)
{
	try
	{
		if (minNbParams((int)_msg->_paramsSplit.size(), 1) == false)
			throw ( ERR_NEEDMOREPARAMS(_client->getAddress(), _client->getNickname(), "topic") );

		std::string	channelName = _msg->_paramsSplit[0];
		if (_server->channelExists(channelName) == false)
			throw ( ERR_NOSUCHCHANNEL(_client->getAddress(), _client->getNickname(), channelName));

		std::vector<Channel*>::iterator	channel = _server->getChannelByName(channelName);
		if ( (*channel)->isUserPresent(_client) == false)
			throw ( ERR_NOTONCHANNEL(_client->getAddress(), _client->getNickname(), (*channel)->getName()) );

		if ( _msg->_paramsSplit.size() == 1)
			sendTopic(*channel);
		else
			modifyTopic(*channel);
	}
	catch(const std::string& message) {
		_client->sendMessage(message);
	}
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */

void	CommandExec::sendTopic(Channel* channel)
{
	std::string message = "";

	if ( channel->getTopic().empty() )
		message = RPL_NOTOPIC(_client->getAddress(), _client->getNickname(), channel->getName());
	else
		message = RPL_TOPIC(_client->getNickname(), channel->getName(), channel->getTopic());

	_client->sendMessage(message);
}

void	CommandExec::modifyTopic(Channel* channel)
{
	if ( channel->getModeT() == true && channel->isOperator(_client) == false)
		throw ( ERR_CHANOPRIVSNEEDED(channel->getName()) );

	std::string	topic = getFullMessage();

	if (topic == ":")
		channel->setTopic("");
	else
		channel->setTopic(topic);

	channel->sendMessageToUsers(RPL_SET_TOPIC(_client, channel->getName(), channel->getTopic()));
}
