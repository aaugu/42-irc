/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/12 15:46:02 by lvogt            ###   ########.fr       */
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

std::vector<std::string> split(std::string value) {
    std::istringstream iss(value);
    std::vector<std::string> mots;
    std::string mot;

    while (iss >> mot)
        mots.push_back(mot);
    return mots;
}

bool checkUseNickname(Server *s, std::string &nickname) {
    std::vector<std::string> nick = s->getNicknameList();
    std::vector<std::string>::iterator it;

    for (it = nick.begin(); it != nick.end(); ++it) {
        if (*it == nickname)
            return true;
    }
    return false;
}

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Client::Client(int sockfd) : _sockfd(sockfd), _isOp(false) {
    std::cout << "coucou" << std::endl;
}

Client::~Client(void) {
    std::cout << "bye bye" << std::endl;
}

/* ************************************************************************** */
/*                           PRIVATE MEMBER FUNCTION                           */
/* ************************************************************************** */

// Class function
void Client::nickFunction(Server *s, std::vector<std::string> &data) {
    bool dupe = false;

    std::vector<std::string> serverNickname = s->getNicknameList();
    while (checkUseNickname(s, data[0])) {
        std::cout << "DUPE" <<std::endl;
        data[0] += '_';
        dupe = true;
    }
    if (dupe) {
        std::cout << "change nickname" << std::endl;
        std::string nickChangeMessage = ": NICK " + data[0];
        sendMessage(nickChangeMessage, _sockfd);
        sendMessage("Nickname already used, Nickname changed to " + data[0], _sockfd);
    }
    _nickname = data[0];
}

void Client::setOperatorState(Server *s, std::string givenPassword) {

    if (givenPassword == s->getOpPass()) {
        if (!_isOp) {
            sendMessage("You are now operator", _sockfd);
            _isOp = true;
            return;
        }
        sendMessage("You are no longer operator", _sockfd);
        _isOp = false;
        return;
    }
    sendMessage("ERROR : Invalid password", _sockfd);
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

void Client::splitMessage(std::string buff) {
    std::stringstream ss(buff);
    std::string word;
    int count = 0;
    _message._paramsSplit.clear();
    _message._params.clear();
    while (ss >> word) {
        if (count == 0)
            _message._command = word;
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

void	Client::send_to(std::string text) const {
	send(_sockfd, text.c_str(), text.length(), 0);
}

void Client::saveMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
    std::cout << "_message._fullStr \"" << _message._fullStr << "\"" << std::endl;
}

void Client::exeCommand(Server *s) {
    std::string type[] = {"PASS", "NICK", "USER", "JOIN", "OPER"}; //ajout d'autre commande
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
            // command_pass();
            std::cout << "TO DO PASS OF \"" << _message._params << "\"" << std::endl;
            break;
        case 1:
            // command_nick();
            std::cout << "TO DO NICK OF \"" << _message._params << "\"" << std::endl;
            nickFunction(s, _message._paramsSplit);
            //_nickname = _message._params;
            send_to(MSG_WELCOME(_nickname, "_user", "_hostName"));
            break;
        case 2:
            // command_user();
            std::cout << "TO DO USER OF \"" << _message._params << "\"" << std::endl;
            break;
        case 3:
            // command_join();
            std::cout << "TO DO JOIN OF \"" << _message._params << "\"" << std::endl;
            if(_message._params.compare(":") == 0){
                send_to(MSG_FIRSTJOIN(_nickname));
            }
            break;
        case 4:
            setOperatorState(s, _message._paramsSplit[1]);
            break;
    }
}

void Client::parseMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
    std::cout << "Client " << _sockfd << ": " << _message._fullStr << std::endl;;
    splitMessage(_message._fullStr);
    _message._fullStr.erase();
    std::cout << "_message._fullStr aftersplit\"" << _message._fullStr << "\"" << std::endl;
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