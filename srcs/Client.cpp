/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/18 11:47:27 by lvogt            ###   ########.fr       */
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

// Client::Client(void) {}

Client::Client(int sockfd) : _sockfd(sockfd) {
    std::cout << "coucou" << std::endl;
    _passwordReceved = false;
    _passwordChecked = false;
    _welcomSended = false;
}

Client::~Client(void) {
    std::cout << "bye bye" << std::endl;
}

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
            sendMessage(nickChangeMessage, _sockfd);
            sendMessage("Nickname changed to " + info[1], _sockfd);
        }
        _nickname = info[1];
    }
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

// void Client::setFd(int value) {
//     _sockfd = value;
// }

void Client::setNickname(std::string value) {
    _nickname = value;
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

void Client::exeCommand(Server &server, std::vector<pollfd>::iterator pollfd) {
    std::string type[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "PING", "QUIT"}; //ajout d'autre commande 
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
            command_pass(server, pollfd);
            break;
        case 1:
            // command_nick();
            std::cout << "TO DO NICK OF \"" << _message._params << "\"" << std::endl;
            check_if_pass(server, pollfd);
            _nickname = _message._params;
            if (_passwordReceved == true && _passwordChecked == true && _welcomSended == false){
                send_to(RPL_WELCOME(_nickname, "_user", "_hostName"));
                _welcomSended = true;
            }
            break;
        case 2:
            // command_user();
            check_if_pass(server, pollfd);
            std::cout << "TO DO USER OF \"" << _message._params << "\"" << std::endl;
            break;
        case 3:
            // command_join();
            std::cout << "TO DO JOIN OF \"" << _message._params << "\"" << std::endl;
            if(_message._params.compare(":") == 0 && _passwordReceved == false){
                send_to(ERR_NOTREGISTERED(_nickname));
                break;
            }
            check_if_pass(server, pollfd);
            break;
        case 4:
            check_if_pass(server, pollfd);
            std::cout << "TO DO MODE OF \"" << _message._params << "\"" << std::endl;
            // command_mode();
            break;
        case 5:
            check_if_pass(server, pollfd);
            std::cout << "TO DO PING OF \"" << _message._params << "\"" << std::endl;
            command_ping();
            break;
        case 6:
            std::cout << "TO DO QUIT OF \"" << _message._params << "\"" << std::endl;
            command_quit(server, pollfd);
            break;
        case 7: //dernier case pour l'invalide command 
            send_to(ERR_INVALID_ERROR);
        // case X: 
        //      ...
    }
}

void Client::parseMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
    std::cout << "Client " << _sockfd << ": " << _message._fullStr << std::endl;;
    splitMessage(_message._fullStr);
    _message._fullStr.erase();
    std::cout << "_message._fullStr aftersplit\"" << _message._fullStr << "\"" << std::endl;
}

void Client::command_pass(Server &server, std::vector<pollfd>::iterator pollfd) {
    _passwordReceved = true;
    if (_message._params.compare(server.get_password()) == 0 && _passwordChecked == false) {
        _passwordChecked = true;
        send_to("Password Accepted\r\n");
    }
    check_if_pass(server, pollfd);
}

void Client::check_if_pass(Server &server, std::vector<pollfd>::iterator pollfd) {
    if (_passwordReceved == false) {
        send_to(ERR_PASSWDMISS);
        server.disconnectClient(pollfd);
    }
    else if (_passwordChecked == false) {
        send_to(ERR_PASSWDMISMATCH);
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
        send_to(ERR_NOORIGIN(_message._command));
        return;
    }
    else
        send_to(PONG(_message._params));
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