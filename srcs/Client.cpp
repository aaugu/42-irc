/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/18 15:29:23 by lvogt            ###   ########.fr       */
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

void toUpperCase(std::string& str) {
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        *it = std::toupper(static_cast<unsigned char>(*it));
    }
}

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

// Class function
bool Client::checkUseNickname(Server *s, std::string &nickname) {
       std::vector<std::string> nick = s->getNicknameList();
    std::vector<std::string>::iterator it;

    for (it = nick.begin(); it != nick.end(); ++it) {
        if (*it == nickname)
            return true;
    }
    return false;
}

std::string Client::nickFunction(Server *s, std::string nickname) {
    bool dupe = false;

    if (nickname[0] == '#') {
        nickname.erase(0, 1);
        sendMessage("Nickname can't sttart with '#', we have remove it for you\r\n");
    }

    while (checkUseNickname(s, nickname)) {
        nickname += '_';
        dupe = true;
    }
    if (dupe) {
        std::string nickChangeMessage = ": NICK " + nickname + "\r\n";
        sendMessage(nickChangeMessage);
        sendMessage("Nickname already used, Nickname changed to " + nickname + "\r\n");
    }
    return nickname;
}




void Client::saveMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
}

void Client::getUserCmdInfo() {
    _username = _message._paramsSplit[0];
    _address = _message._paramsSplit[2];

}

void Client::exeCommand(Server* server)
{
    CommandExec exec(server, this, &_message); //pour le join pour le moment

    std::string type[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "PING", "QUIT", "CAP", "OPER", "KILL"}; //ajout d'autre commande
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
            command_pass(*server);
            break;
        case 1:
            // command_nick();
            std::cout << "TO DO NICK OF \"" << _message._params << "\"" << std::endl;
            check_if_pass(*server);
            _nickname = nickFunction(server, _message._params);
            if (_passwordReceved == true && _passwordChecked == true && _welcomSended == false){
                sendMessage(RPL_WELCOME(_nickname, "_user", "_hostName"));
                _welcomSended = true;
            }
            break;
        case 2:
            // command_user();
            check_if_pass(*server);
            std::cout << "TO DO USER OF \"" << _message._params << "\"" << std::endl;
            getUserCmdInfo();
            std::cout << "DEBUG => " << _username << " : " << _address << std::endl;
            break;
        case 3:
            // command_join();
            std::cout << "TO DO JOIN OF \"" << _message._params << "\"" << std::endl;
            if(_message._params.compare(":") == 0 && _passwordReceved == false){
                sendMessage(ERR_NOTREGISTERED(_nickname));
                break;
            }
            check_if_pass(*server);
            exec.join();
            break;
        case 4:
            check_if_pass(*server);
            std::cout << "TO DO MODE OF \"" << _message._params << "\"" << std::endl;
            // command_mode();
            break;
        case 5:
            check_if_pass(*server);
            std::cout << "TO DO PING OF \"" << _message._params << "\"" << std::endl;
            command_ping();
            break;
        case 6:
            std::cout << "TO DO QUIT OF \"" << _message._params << "\"" << std::endl;
            command_quit(*server);
            break;
        case 7: //autorisation de la commande CAP mais on fait rien avec pour le moment
            break;
        case 8:
            check_if_pass(*server);
            exec.oper();
            break;
        case 9:
            check_if_pass(*server);
            //killClient(server, _message._paramsSplit);
            break;
        default: //dernier case pour l'invalide command 
            sendMessage(ERR_INVALID_ERROR);
        // case X: 
        //      ...
    }
}

void Client::eraseFullstr(void) {
    splitMessage(_message._fullStr);
    _message._fullStr.erase();
    std::cout << "_message._fullStr erased" << std::endl;
}

void Client::parseMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
    std::cout << "Client " << _sockfd << ": " << _message._fullStr << std::endl;
    splitMessage(_message._fullStr);
    eraseFullstr();
    std::cout << "_message._fullStr aftersplit\"" << _message._fullStr << "\"" << std::endl;
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

void    Client::setCurrentChannel(Channel* currentChannel) {
    _currentChannel = currentChannel;
}

void    Client::setOperatorState(bool value) {
    _isOp = value;
}

/* ************************************************************************** */
/*                             PRIVATE FUNCTIONS                              */
/* ************************************************************************** */

void Client::command_pass(Server &server) {
    _passwordReceved = true;
    if (_message._params.compare(server.get_password()) == 0 && _passwordChecked == false) {
        _passwordChecked = true;
        sendMessage("Password Accepted\r\n");
    }
    check_if_pass(server);
}

void Client::check_if_pass(Server &server) {
    if (_passwordReceved == false) {
        sendMessage(ERR_PASSWDMISS);
        server.disconnectClient(this);
    }
    else if (_passwordChecked == false) {
        sendMessage(ERR_PASSWDMISMATCH);
        server.disconnectClient(this);
    }
}

void Client::command_quit(Server &server) {
    //envoyer un message "Machin" + _message._params 
    // à tout les utilisateurs des channels de Machin
    server.disconnectClient(this);
}

void Client::command_ping(void) {
    if (_message._params.empty()) {
        sendMessage(ERR_NOORIGIN(_message._command));
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
