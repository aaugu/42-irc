/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/11 13:05:33 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/SendMessages.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

// Client::Client(void) {}

Client::Client(int sockfd) : _sockfd(sockfd) {
    std::cout << "coucou" << std::endl;
}

Client::~Client(void) {
    std::cout << "bye bye" << std::endl;
}

/* ************************************************************************** */
/*                           PUBLIC MEMBER FUNCTION                           */
/* ************************************************************************** */

// Class function
void Client::setData(char *buffer) {
    (void) buffer;
    // std::string test = buffer;
    // // std::vector<std::string> res;
    // // std::istringstream f(test);
    // // std::string s;
    // // while (getline(f, s, "\r\n")) {
    // //     if (s.rfind("NICK", 0) == 0) {
    // //         std::cout << "'"<< buffer << "'" << std::endl;
    // //         _nickname = s.substr(5, 6);
    // //     }
    // // }
    // std::cout << "va chier '" << t(test) << std::endl;
    // if (test.find("NICK", 0) == 0) {
    //     std::cout << "'" << t(test) << "'" << std::endl;
    //     std::cout << "conard " << test.substr(5) << std::endl;
    // }
    //std::cout << "debug client nickname : " << _nickname << ", fd : " << _fd << " ." << std::endl;
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

void Client::setFd(int value) {
    _sockfd = value;
}

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

void Client::parseMessage(std::string buff) {
    splitMessage(buff);
    std::string type[] = {"PASS", "NICK", "USER", "JOIN"}; //ajout d'autre commande 
    int count = 0;
    size_t arraySize = sizeof(type) / sizeof(type[0]);
    // const char* response;
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
        // case 4:
        //     ...
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