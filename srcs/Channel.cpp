/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:58 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/26 11:02:34 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <unistd.h>
#include <string>

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/messages.hpp"
#include "../includes/Server.hpp"

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
	_users.insert(std::pair<Client*, bool>(user, true));

	(void) _modeT;
	(void) _modeK;
	(void) _modeL;
}

Channel::~Channel(void) {}

/* ************************************************************************** */
/*                          PUBLIC MEMBER FUNCTIONS                           */
/* ************************************************************************** */

void	Channel::addUser(Client* user, bool isOperator)
{
	if (user)
		_users.insert(std::pair<Client*, bool>(user, isOperator));
	else
		printErrMessage(errMessage("User", user->getFd(), "could not add unknown user to channel"));
}

int	Channel::removeUser(Client* user)
{
	std::map<Client*, bool>::iterator userFound = _users.find(user);
	if ( userFound != _users.end())
	{
		_users.erase(user);
		user->sendMessage(":You have left " + this->_name + "\r\n");
	}
	else
		printErrMessage(errMessage("User", user->getFd(), "could not remove unknown user of channel"));
	return ( _users.size() );
}

bool	Channel::isOperator(Client* user)
{
	std::map<Client*, bool>::iterator userFound = _users.find(user);
	if ( userFound != _users.end())
		return (userFound->second);
	else
		return (false);
}

bool	Channel::isPasswordValid(std::string password) {
	return ( _password == password );
}

bool	Channel::isUserPresent(Client* client)
{
	if ( _users.find(client) != _users.end() )
		return (true);
	return (false);
}

void	Channel::sendMessageToUsers(std::string message)
{
	std::map<Client*, bool>::iterator user;
	for( user = _users.begin(); user != _users.end(); user++ )
		user->first->sendMessage(message);
}

void	Channel::sendMessageToUsersExceptSender(Client* sender, std::string message)
{
	std::map<Client*, bool>::iterator user;
	for( user = _users.begin(); user != _users.end(); user++ )
	{
		if (user->first != sender)
			user->first->sendMessage(message);
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

std::map<Client*, bool>	Channel::getUsers(void) {
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

	std::map<Client*, bool>::iterator user;
	for( user = _users.begin(); user != _users.end(); user++ )
	{
		if (isOperator(user->first) == true)
			userList += "@" + user->first->getNickname() + " ";
		else
			userList += user->first->getNickname() + " ";
	}

	return ( userList );
}
