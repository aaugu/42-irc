/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverChannelUtils.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:34:08 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/17 19:19:26 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

/* ************************************************************************** */
/*                                CHANNEL UTILS                               */
/* ************************************************************************** */

void	Server::addChannel(Channel& channel)
{
	_channels.push_back(channel);
	// message sur serveur pour dire qu'on a crée un channel
}

void	Server::removeChannel(std::vector<Channel>::iterator channel)
{
	(void) channel;
	// _channels.push_back(channel);
	// message sur serveur pour dire qu'un channel a été fermé
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
