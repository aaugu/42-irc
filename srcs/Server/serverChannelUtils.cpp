/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverChannelUtils.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:34:08 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/26 10:45:49 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/messages.hpp"

/* ************************************************************************** */
/*                                CHANNEL UTILS                               */
/* ************************************************************************** */

void	Server::addChannel(Channel& channel)
{
	_channels.push_back(channel);
}

bool	Server::channelExists(std::string name)
{
	std::vector<Channel>::iterator it;
	for( it = _channels.begin(); it < _channels.end(); it++)
	{
		if ( it->getName() == name)
			return (true) ;
	}
	return (false);
}

std::vector<Channel>::iterator	Server::getChannelByName(std::string name)
{
	std::vector<Channel>::iterator it;
	for( it = _channels.begin(); it < _channels.end(); it++)
	{
		if ( it->getName() == name)
			break ;
	}
	return (it);
}

void	Server::closeChannel(std::string name)
{
	if ( channelExists(name))
	{
		std::vector<Channel>::iterator channel = getChannelByName(name);
		_channels.erase(channel);
		std::cout << "Channel " << name << " closed\n";
	}
	else
		printErrMessage(errMessage(name, -1, "Channel not found"));
}
