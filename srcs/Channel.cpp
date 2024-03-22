/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:58 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/22 14:31:35 by lvogt            ###   ########.fr       */
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

void	Channel::removeUser(Client* user, Server* server)
{
	std::map<Client*, bool>::iterator userFound = _users.find(user);
	if ( userFound != _users.end())
	{
		_users.erase(user);
		user->sendMessage("You have left " + this->_name + "\r\n");
		if ( _users.size() == 0)
			server->closeChannel(this->_name);
	}
	else
		printErrMessage(errMessage("User", user->getFd(), "could not remove unknown user of channel"));

}

bool	Channel::isOperator(Client* user)
{
	std::map<Client*, bool>::iterator userFound = _users.find(user);
	if ( userFound != _users.end())
		return (userFound->second);
	else
		return (false);
}

void	Channel::setOperator(Client* user, bool oper)
{
	std::map<Client*, bool>::iterator userFound = _users.find(user);
	if ( userFound != _users.end())
		userFound->second = oper;
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

void	Channel::setPassword(std::string password) {
	_password = password;
}

int	Channel::getUserLimit(void) {
	return ( _userLimit );
}

void	Channel::setUserLimit(int limit) {
	_userLimit = limit;
}

std::map<Client*, bool>	Channel::getUsers(void) {
	return ( _users );
}

bool	Channel::getModeI(void) {
	return ( _modeI );
}

bool	Channel::getModeT(void) {
	return ( _modeT );
}

bool	Channel::getModeK(void) {
	return ( _modeK );
}

bool	Channel::getModeL(void) {
	return ( _modeL );
}

void	Channel::setModeI(bool newmode) {
	_modeI = newmode;
}

void	Channel::setModeT(bool newmode) {
	_modeT = newmode;
}

void	Channel::setModeK(bool newmode) {
	_modeK = newmode;
}

void	Channel::setModeL(bool newmode) {
	_modeL = newmode;
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