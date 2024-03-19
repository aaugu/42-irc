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
        sendMessage(nickChangeMessage);
        sendMessage("Nickname already used, Nickname changed to " + data[0]);
    }
    _nickname = data[0];
}

void Client::setOperatorState(Server *s, std::vector<std::string> args) {
    std::vector<Client>::iterator itC = s->getClientByNickname(args[0]);
    if (args[1] == s->getOpPass()) {
        if (!_isOp) {
            if (_nickname == args[0]) {
                sendMessage("You are now operator of this server\r\n");
                itC->_isOp = true;
                return;
            }
            else {
                sendMessage("ERROR : You need to be operator to change permission of other user\r\n");
                return;
            }
        }
        else {

			if (itC->getFd() == 0 || itC->getFd() == 49) { // fd == 49 error if no other user connected in wsl (TODO tester sous mac)
				sendMessage("ERROR : Invalid user, please enter a valide nickname\r\n");
				return;
			}

            if (!itC->_isOp) {
                sendMessage( "You have promoted " + itC->_nickname + " Operator\r\n");
                sendMessageTo("You have been promoted Operator by " + _nickname + "\r\n", itC->_sockfd);
                itC->_isOp = true;
                return;
            }
            else {
				if (_nickname == args[0]){
					sendMessage("You are no longer operator\r\n");
				}else {
					sendMessage( itC->_nickname + " is no longer operator\r\n");
					sendMessageTo(_nickname + " have removed your permission\r\n", itC->_sockfd);
				}
                itC->_isOp = false;
                return;
            }
        }
    }
    sendMessage("ERROR : Invalid password");
}

void    Client::killClient(Server *s, std::vector<std::string> args) {
    if (!_isOp) {
        sendMessage("ERROR : You need to be operator to kill a client\r\n");
        return;
    }

    if (s->checkClientPresence(args[0])) {
        std::vector<Client>::iterator itC = s->getClientByNickname(args[0]);
        if (args[1] == ":") {
            args[1] += "unknown reason";
        }
        sendMessageTo("You have been kicked by " + _nickname  + " " + args[1] + "\r\n", itC->getFd());
        s->disconnectClient(&(*itC));
        sendMessage(itC->getNickname() + " has been disconnected\r\n");
    } else {
        sendMessage("ERROR : it's not a valid user\r\n");
    }
}


void Client::saveMessage(std::string buff) {
    _message._fullStr = _message._fullStr + buff;
    std::cout << "_message._fullStr \"" << _message._fullStr << "\"" << std::endl;
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
            _nickname = _message._params;
            if (_passwordReceved == true && _passwordChecked == true && _welcomSended == false){
                sendMessage(RPL_WELCOME(_nickname, "_user", "_hostName"));
                _welcomSended = true;
            }
            break;
        case 2:
            // command_user();
            check_if_pass(*server);
            std::cout << "TO DO USER OF \"" << _message._params << "\"" << std::endl;
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
            std::cout << "DEBUG" << std::endl;
            check_if_pass(*server);
            setOperatorState(server, _message._paramsSplit);
            break;
        case 9:
            check_if_pass(*server);
            killClient(server, _message._paramsSplit);
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

void Client::setNickname(std::string value) {
    _nickname = value;
}

void    Client::setCurrentChannel(Channel* currentChannel) {
    _currentChannel = currentChannel;
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
