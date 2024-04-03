/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/03 15:25:07 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>
#include <unistd.h>

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/messages.hpp"
#include "../includes/SendMessages.hpp"
#include "../includes/CommandExec.hpp"

#define DEFAULTNICKNAME "G'raha Tia"
#define DEFAULTUSERNAME "Meteor"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Client::Client(int sockfd, std::string address) :
                _sockfd(sockfd),
                _nickname(DEFAULTNICKNAME),
                _username(DEFAULTUSERNAME),
                _isOp(false),
                _address(address),
                _passwordReceved(false),
                _passwordChecked(false),
                _welcomSended(false) {}

Client::~Client(void) {}

/* ************************************************************************** */
/*                           PRIVATE MEMBER FUNCTION                           */
/* ************************************************************************** */

// Utils

void toUpperCase(std::string& str) {
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        *it = std::toupper(static_cast<unsigned char>(*it));
    }
}

void Client::splitMessage(std::string buff) {
    std::stringstream ss(buff);
    std::string word;
    int count = 0;
    _message._paramsSplit.clear();
    _message._params.clear();
    while (ss >> word) {
        if (count == 0) {
            toUpperCase(word);
            _message._command = word;
        }
        else if (count == 1)
        {
            _message._paramsSplit.push_back(word);
            _message._params = word;
        }
        else if (count > 1)
        {
            _message._paramsSplit.push_back(word);
            _message._params += " " + word;
        }
        count++;
    }
}

// Gestion signal CTRL-D

void Client::saveMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
}

void Client::getUserCmdInfo() {
    //TODO Gris?
    _username = _message._paramsSplit[0];
    _address = _message._paramsSplit[2];

}

void Client::eraseFullstr(void) {
    splitMessage(_message._fullStr);
    _message._fullStr.erase();
}

void Client::parseMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
    splitMessage(_message._fullStr);
    eraseFullstr();
}

// commande execution

void Client::exeCommand(Server* server)
{
    CommandExec exec(server, this, &_message);


    std::string type[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "PING", "QUIT", "CAP", "OPER", "KILL", "PRIVMSG", "PART", "INVITE", "WHO", "WHOIS", "TOPIC"};
    int count = 0;
    size_t arraySize = sizeof(type) / sizeof(type[0]);
    for (int i = 0; i < (int)arraySize; i++){
        if (_message._command.compare(type[i]) != 0)
            count++;
        else
            break;
    }

    switch (count) {
        case 0:
            std::cout << "TO DO PASS OF \"" << _message._params << "\"" << std::endl;
            exec.pass();
            break;
        case 1:
            std::cout << "TO DO NICK OF \"" << _message._params << "\"" << std::endl;
            exec.check_if_pass();
            exec.nick();

            break;
        case 2:
            exec.check_if_pass();
            std::cout << "TO DO USER OF \"" << _message._params << "\"" << std::endl;
            getUserCmdInfo();
            break;
        case 3:
            if(_message._params.compare(":") == 0 && _passwordReceved == false){
                sendMessage(ERR_NOTREGISTERED(_nickname));
                break;
            }
            exec.check_if_pass();
            std::cout << "TO DO JOIN OF \"" << _message._params << "\"" << std::endl;
            exec.join();
            break;
        case 4:
            exec.check_if_pass();
            std::cout << "TO DO MODE OF \"" << _message._params << "\"" << std::endl;
            exec.mode();
            break;
        case 5:
            exec.check_if_pass();
            std::cout << "TO DO PING OF \"" << _message._params << "\"" << std::endl;
            exec.ping();
            break;
        case 6:
            std::cout << "TO DO QUIT OF \"" << _message._params << "\"" << std::endl;
            exec.quit();
            break;
        case 7: //autorisation de la commande CAP mais on fait rien avec pour le moment
            break;
        case 8:
            exec.check_if_pass();
            std::cout << "TO DO OPER OF \"" << _message._params << "\"" << std::endl;
            exec.oper();
            break;
        case 9:
            exec.check_if_pass();
            std::cout << "TO DO KILL OF \"" << _message._params << "\"" << std::endl;
            exec.kill();
            break;
        case 10:
            exec.check_if_pass();
            std::cout << "TO DO PRIVMSG OF \"" << _message._params << "\"" << std::endl;
            exec.privmsg();
            break ;
        case 11:
            exec.check_if_pass();
            std::cout << "TO DO PART OF \"" << _message._params << "\"" << std::endl;
            exec.part();
            break ;
        case 12:
            exec.check_if_pass();
            std::cout << "TO DO INVITE OF \"" << _message._params << "\"" << std::endl;
            exec.invite();
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            exec.check_if_pass();
            std::cout << "TO DO TOPIC OF \"" << _message._params << "\"" << std::endl;
            exec.topic();
            break;
        default: //dernier case pour l'invalide command
            sendMessage(ERR_INVALID_ERROR);
        // case X: 
        //      ...
    }
}

/* ************************************************************************** */
/*                                 ACCESSORS                                  */
/* ************************************************************************** */

int Client::getFd() {
    return _sockfd;
}

std::string Client::getNickname(void) {
    return _nickname;
}

std::string	Client::getAddress(void) {
    return ( _address );
}

bool    Client::getOperatorState() {
    return _isOp;
}

void    Client::setOperatorState(bool value) {
    _isOp = value;
}

bool Client::isPasswordReceved() {
    return _passwordReceved;
}

bool Client::isPasswordChecked() {
    return _passwordChecked;
}

bool Client::isWelcomSended() {
    return _welcomSended;
}

void Client::setWelcomSended(bool welcomSended) {
    _welcomSended = welcomSended;
}

void Client::setPasswordReceved(bool passwordReceved) {
    _passwordReceved = passwordReceved;
}

void Client::setPasswordChecked(bool passwordChecked) {
    _passwordChecked = passwordChecked;
}

void Client::setNickname(std::string nickname) {
    _nickname = nickname;
}

/* ************************************************************************** */
/*                             PRIVATE FUNCTIONS                              */
/* ************************************************************************** */
