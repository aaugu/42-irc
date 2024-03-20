/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExec.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:53:34 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/20 18:49:31 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

#define ERR_NB_ARGS(command, min, max) ("Command: " + command + ": Not enough arguments, should be between" + min + " and " + max)

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

CommandExec::CommandExec(Server* server, Client* client, s_message* msg) :
							_server(server),
							_client(client),
							_msg(msg) {}

CommandExec::~CommandExec(void) {}

/* ************************************************************************** */
/*                                    UTILS                                   */
/* ************************************************************************** */

bool	CommandExec::minNbParams(int nbParams, int minNbParams)
{
	if ( nbParams >= minNbParams)
		return (true);
	return (false);
}

std::string	CommandExec::getFullMessage(void)
{
	std::string	message = "";

	std::vector<std::string>::iterator	it = _msg->_paramsSplit.begin() + 1;
	for( ; it < _msg->_paramsSplit.end(); it++)
	{
		if (it + 1 != _msg->_paramsSplit.end())
			message += *it + " ";
		else
			message += *it;
	}
	return (message);
}
