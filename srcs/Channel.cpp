/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:58 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/02 14:43:01 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <map>
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
					_modeT(true),
					_modeK(false),
					_modeL(false)
{
	_users.insert(std::pair<Client*, bool>(user, true));
}

Channel::~Channel(void) {}

/* ************************************************************************** */
/*                                 MODIFIERS                                  */
/* ************************************************************************** */

void	Channel::addUser(Client* client, bool isOperator)
{
	if (client)
		_users.insert(std::pair<Client*, bool>(client, isOperator));
	else
		printErrMessage(errMessage("User", -1, "could not add unknown user to channel"));
}

void	Channel::addUserToWhitelist(Client* client)
{
	if (client)
		_whitelist.push_back(client);
	else
		printErrMessage(errMessage("User", -1, "could not add unknown user to whitelist"));
}

int	Channel::removeUser(Client* user)
{
	
	std::map<Client*, bool>::iterator userFound = _users.find(user);
	if ( userFound != _users.end())
	{
		_users.erase(user);
		user->sendMessage("You have left " + this->_name + "\r\n");
	}
	else
		printErrMessage(errMessage("User", -1, "could not remove unknown user from channel"));
	return ( _users.size() );
}

void	Channel::removeUserFromWhitelist(Client* user)
{
	std::vector<Client*>::iterator it = _whitelist.begin();
	for ( ; it < _whitelist.end(); it++)
	{
		if (*it == user)
		{
			_whitelist.erase(it);
			return ;
		}
	}
	printErrMessage(errMessage("User", -1, "could not remove unknown user from whitelist"));
}

/* ************************************************************************** */
/*                                  CHECKS                                    */
/* ************************************************************************** */

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

bool	Channel::isUserOnWhitelist(Client* client)
{
	std::vector<Client*>::iterator user = _whitelist.begin();
    for( ; user < _whitelist.end(); user++ )
	{
        if ( *user == client )
            return ( true );
    }
	return ( false );
}

/* ************************************************************************** */
/*                               SEND MESSAGES                                */
/* ************************************************************************** */

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
/*                                  GETTERS                                   */
/* ************************************************************************** */

Client* Channel::getMapptrClientByName(std::string nickname) {
    std::map<Client*, bool>::iterator user;
    for( user = _users.begin(); user != _users.end(); user++ ) {
        if (user->first->getNickname() == nickname)
            return user->first;
    }
    return NULL;
}

std::string	Channel::getName(void) {
	return ( _name );
}

std::string	Channel::getPassword(void) {
	return ( _password );
}

std::string	Channel::getTopic(void) {
	return ( _topic );
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
	return ( _modeT );
}

bool	Channel::getModeK(void) {
	return ( _modeK );
}

bool	Channel::getModeL(void) {
	return ( _modeL );
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

/* ************************************************************************** */
/*                                  SETTERS                                   */
/* ************************************************************************** */

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

void	Channel::setPassword(std::string password) {
	_password = password;
}

void	Channel::setTopic(std::string topic) {
	_topic = topic;
}

void	Channel::setUserLimit(int limit) {
	_userLimit = limit;
}