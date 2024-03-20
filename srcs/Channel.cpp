/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:58 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/20 14:27:32 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <unistd.h>
#include <string>

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/messages.hpp"


/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Channel::Channel(std::string name, Client* user) :
					_name(name),
					_password(""),
					_topic(""),
					_userLimit(-1),
					_modeI(false),
					_modeT(false),
					_modeK(false),
					_modeL(false)
{
	_users.push_back(user);
	_operators.push_back(user);

	(void) _modeT;
	(void) _modeK;
	(void) _modeL;
}

Channel::~Channel(void) {}

/* ************************************************************************** */
/*                          PUBLIC MEMBER FUNCTIONS                           */
/* ************************************************************************** */

void	Channel::addUser(Client* user)
{
	if (user)
		_users.push_back(user);
}

void	Channel::removeUser(Client* user) // le paramètre sera probablement plutôt un itérateur
{
	(void) user;
}

bool	Channel::isOperator(Client* user)
{
	std::vector<Client*>::iterator it = _operators.begin();
	for( ; it < _operators.end(); it ++)
	{
		if (user == *it)
			return (true);
	}
	return (false);
}

bool	Channel::isPasswordValid(std::string password) {
	return ( _password == password );
}

bool	Channel::isUserPresent(Client* client)
{
	std::vector<Client*>::iterator user = _users.begin();
	for( ; user < _users.end(); user++ )
	{
		if (*user == client)
			return (true);
	}
	return (false);
}

void	Channel::sendMessageToUsers(std::string message)
{
	std::vector<Client*>::iterator user;
	for( user = _users.begin(); user < _users.end(); user ++ )
		(*user)->sendMessage(message);
}

void	Channel::sendMessageToUsersExceptSender(Client* sender, std::string message)
{
	std::vector<Client*>::iterator user;
	for( user = _users.begin(); user < _users.end(); user++ )
	{
		if (*user != sender)
			(*user)->sendMessage(message);
	}
}

/* ************************************************************************** */
/*                                 ACCESSORS                                  */
/* ************************************************************************** */

std::string	Channel::getName(void) {
	return ( _name );
}

std::string	Channel::getPassword(void) {
	return ( _password );
}

int	Channel::getUserLimit(void) {
	return ( _userLimit );
}

std::vector<Client*>	Channel::getUsers(void) {
	return ( _users );
}

bool	Channel::getModeI(void) {
	return ( _modeI );
}

bool	Channel::getModeT(void) {
	return ( _modeI );
}

bool	Channel::getModeK(void) {
	return ( _modeI );
}

bool	Channel::getModeL(void) {
	return ( _modeI );
}

std::string	Channel::getAllUsersList(void)
{
	std::string userList = "";

	std::vector<Client*>::iterator user;
	for ( user = _users.begin(); user < _users.end(); user++)
	{
		if (isOperator(*user) == true)
			userList += "@" + (*user)->getNickname() + " ";
		else
			userList += (*user)->getNickname() + " ";
	}

	return ( userList );
}
