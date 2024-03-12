/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/12 12:19:31 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <vector>
#include "../includes/Server.hpp"
#include "../includes/messages.hpp"

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

Client::Client(int sockfd) : _sockfd(sockfd) {}

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