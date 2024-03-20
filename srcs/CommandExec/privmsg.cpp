/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:32:28 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/19 16:59:21 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "../includes/CommandExec.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// RESPONSES
#define RPL_PRIVMSG(name, message) (name + " " + message)
												// shoud be username
// #define USER(user) (user->getNickname() + "!" + user->getNickname() + "@" + user->getAddress())
// #define RPL_JOIN(client, channel) (":" + USER(client) + " JOIN " + channel + "\r\n")
// #define RPL_ENDOFNAMES(address, client, channel) (":" + address + " 366 " + client + " " + channel + " :End of /NAMES list\r\n")
// #define RPL_NAMREPLY(address, client, channel, nickname) (":" + address + " 353 " + client + " = " + channel + " :" + nickname + "\r\n")

// ERRORS
#define ERR_NOSUCHCHANNEL(address, client, channel) (":" + address + " 403 " + client + " " + channel + ":Channel name is invalid, or does not exist\r\n")
#define ERR_NOSUCHNICK(address, client)  (":" + address + " 401 " + client + " " + ":Nickname is invalid, or does not exist\r\n")
// #define ERR_CHANNELISFULL(address, client, channel)  (":" + address + " 471 " + client + " " + channel + ":Cannot join channel (+l)\r\n")

/* ************************************************************************** */
/*                                 PRIVMSG                                    */
/* ************************************************************************** */

void	CommandExec::privmsg(void)
{
	if (invalidNbParams((int)_msg->_paramsSplit.size(), 2, 2))
		return (_client->sendMessage("privmsg nb arg \r\n"));

	std::string	target = _msg->_paramsSplit[0];
	if (target[0] == '#')
	{
		std::cout << "Channel " << target << std::endl;
		sendMessageToChannel(target, RPL_PRIVMSG(_client->getNickname(), _msg->_paramsSplit[1]));
	}
	else
	{
		std::cout << "Msg prive user " << target << std::endl;
		sendPrivateMessage(target, RPL_PRIVMSG(_client->getNickname(), _msg->_paramsSplit[1]));
	}
}

void CommandExec::sendMessageToChannel(std::string target, std::string message)
{
	if (_server->channelExists(target))
	{
		std::cout << "channel existant\n";
		std::vector<Channel>::iterator	channel = _server->getChannelByName(target);
		std::cout << "je veux envoyer dans " << channel->getName() << std::endl;
		channel->sendMessageToUsersExceptSender(_client, message);
	}
	else
		_client->sendMessage(ERR_NOSUCHCHANNEL(_client->getAddress(), _client->getNickname(), target));
}

void CommandExec::sendPrivateMessage(std::string target, std::string message)
{
	if (_server->clientExists(target))
	{
		std::cout << "user existant\n";
		std::vector<Client>::iterator	user = _server->getClientByNickname(target);
		std::cout << "je veux envoyer a " << user->getNickname() << std::endl;
		user->sendMessage(message);
	}
	else
		_client->sendMessage(ERR_NOSUCHNICK(_client->getAddress(), target));
}