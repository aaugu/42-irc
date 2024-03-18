/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExec.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:53:34 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/18 14:06:13 by aaugu            ###   ########.fr       */
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

bool	CommandExec::invalidNbParams(int nbParams, int minNbParams, int maxNbParams)
{
	if ( nbParams < minNbParams || nbParams > maxNbParams)
		return (true);
	return (false);
}
