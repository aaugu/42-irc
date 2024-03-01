/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:43:23 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/01 13:54:03 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <algorithm>
#include "../includes/Client.hpp"

/* ************************************************************************** */
/*                           ORTHODOX CANONICAL FORM                          */
/* ************************************************************************** */

Client::Client(void) {}

Client::Client(int sockfd) : _sockfd(sockfd) {}

// TO DO : Constructeur par copie + operateur de surcharge =

Client::~Client(void) {}

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