/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/18 13:51:08 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/messages.hpp"
#include "../includes/SendMessages.hpp"
#include "../includes/CommandExec.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Client::Client(int sockfd, std::string address) :
                _sockfd(sockfd),
                _address(address),
                _passwordReceved(false),
                _passwordChecked(false),
                _welcomSended(false) {}

Client::~Client(void) {}

/* ************************************************************************** */
/*                           PUBLIC MEMBER FUNCTION                           */
/* ************************************************************************** */

// Class function
void Client::setData(Server *s, std::string &buffer) {
    std::vector<std::string> info = split(buffer);
    bool dupe = false;

    if (info[0] == "NICK") {
        while (checkUseNickname(s, info[1])) {
            std::cout << "DUPE" <<std::endl;
            info[1] += '_';
            dupe = true;
        }
        if (dupe) {
            std::cout << "change nickname" << std::endl;
            std::string nickChangeMessage = ": NICK " + info[1];
            sendMessage(nickChangeMessage);
            sendMessage("Nickname changed to " + info[1]);
        }
        _nickname = info[1];
    }
}

void Client::saveMessage(std::string buff) {
    _message.fullStr = _message.fullStr + buff;
    std::cout << "_message.fullStr \"" << _message.fullStr << "\"" << std::endl;
}

void Client::exeCommand(Server* server, std::vector<pollfd>::iterator pollfd)
{
    CommandExec exec(server, this, &_message);

    std::string type[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "PING", "QUIT"}; //ajout d'autre commande
    int count = 0;
    size_t arraySize = sizeof(type) / sizeof(type[0]);
    for (int i = 0; i < (int)arraySize; i++){
        if (_message.command.compare(type[i]) != 0)
            count++;
        else
            break;
    }

    switch (count) {
        case 0:
            std::cout << "TO DO PASS OF \"" << _message._params << "\"" << std::endl;
            command_pass(*server, pollfd);
            break;
        case 1:
            // command_nick();
            std::cout << "TO DO NICK OF \"" << _message._params << "\"" << std::endl;
            check_if_pass(*server, pollfd);
            _nickname = _message._params;
            if (_passwordReceved == true && _passwordChecked == true && _welcomSended == false){
                sendMessage(RPL_WELCOME(_nickname, "_user", "_hostName"));
                _welcomSended = true;
            }
            break;
        case 2:
            // command_user();
            check_if_pass(*server, pollfd);
            std::cout << "TO DO USER OF \"" << _message._params << "\"" << std::endl;
            break;
        case 3:
            // command_join();
            std::cout << "TO DO JOIN OF \"" << _message._params << "\"" << std::endl;
            if(_message._params.compare(":") == 0 && _passwordReceved == false){
                sendMessage(ERR_NOTREGISTERED(_nickname));
                break;
            }
            check_if_pass(*server, pollfd);
            exec.join();
            break;
        case 4:
            check_if_pass(*server, pollfd);
            std::cout << "TO DO MODE OF \"" << _message._params << "\"" << std::endl;
            // command_mode();
            break;
        case 5:
            check_if_pass(*server, pollfd);
            std::cout << "TO DO PING OF \"" << _message._params << "\"" << std::endl;
            command_ping();
            break;
        case 6:
            std::cout << "TO DO QUIT OF \"" << _message._params << "\"" << std::endl;
            command_quit(*server, pollfd);
            break;
        default: //dernier case pour l'invalide command 
            sendMessage(ERR_INVALID_ERROR);

    }
}

void Client::parseMessage(std::string buff) {
    _message.fullStr = _message.fullStr + buff;
    std::cout << "Client " << _sockfd << ": " << _message.fullStr << std::endl;;
    splitMessage(_message.fullStr);
    _message.fullStr.erase();
    std::cout << "_message.fullStr aftersplit\"" << _message.fullStr << "\"" << std::endl;
}

/* ************************************************************************** */
/*                                 ACCESSORS                                  */
/* ************************************************************************** */

int Client::getFd() {
    return _sockfd;
}

std::string Client::getNickname() {
    return _nickname;
}

std::string	Client::getAddress(void) {
    return ( _address );
}

void Client::setNickname(std::string value) {
    _nickname = value;
}

void    Client::setCurrentChannel(Channel* currentChannel) {
    _currentChannel = currentChannel;
}

/* ************************************************************************** */
/*                             PRIVATE FUNCTIONS                              */
/* ************************************************************************** */

void Client::command_pass(Server &server, std::vector<pollfd>::iterator pollfd) {
    _passwordReceved = true;
    if (_message._params.compare(server.get_password()) == 0 && _passwordChecked == false) {
        _passwordChecked = true;
        sendMessage("Password Accepted\r\n");
    }
    check_if_pass(server, pollfd);
}

void Client::check_if_pass(Server &server, std::vector<pollfd>::iterator pollfd) {
    if (_passwordReceved == false) {
        sendMessage(ERR_PASSWDMISS);
        server.disconnectClient(pollfd);
    }
    else if (_passwordChecked == false) {
        sendMessage(ERR_PASSWDMISMATCH);
        server.disconnectClient(pollfd);
    }

}

void Client::command_quit(Server &server, std::vector<pollfd>::iterator pollfd) {
    //envoyer un message "Machin" + _message._params 
    // à tout les utilisateurs des channels de Machin
    server.disconnectClient(pollfd);
}

void Client::command_ping(void) {
    if (_message._params.empty()) {
        sendMessage(ERR_NOORIGIN(_message.command));
        return;
    }
    else
        sendMessage(PONG(_message._params));
}

std::vector<std::string> Client::split(std::string value) {
    std::istringstream iss(value);
    std::vector<std::string> mots;
    std::string mot;

    while (iss >> mot)
        mots.push_back(mot);
    return mots;
}

void Client::splitMessage(std::string buff) {
    std::stringstream ss(buff);
    std::string word;
    int count = 0;
    _message._paramsSplit.clear();
    _message._params.clear();
    while (ss >> word) {
        if (count == 0)
            _message.command = word;
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

bool Client::checkUseNickname(Server *s, std::string &nickname) {
    std::vector<std::string> nick = s->getNicknameList();
    std::vector<std::string>::iterator it;

    for (it = nick.begin(); it != nick.end(); ++it) {
        if (*it == nickname)
            return true;
    }
    return false;
}

// /* ************************************************************************** */
// /*                            NON MEMBER FUNCTIONS                            */
// /* ************************************************************************** */

// std::string t(const std::string& input) {
//     std::string result;
//     for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
//         char c = *it;
//         switch (c) {
//             case '\n':
//                 result += "\\n";
//                 break;
//             case '\r':
//                 result += "\\r";
//                 break;
//             case '\t':
//                 result += "\\t";
//                 break;
//             // Ajoutez d'autres caractères spéciaux si nécessaire
//             default:
//                 result += c;
//                 break;
//         }
//     }
//     return result;
// }
