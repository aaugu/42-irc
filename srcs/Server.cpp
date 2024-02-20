/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/20 14:39:18 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <list>
#include <iostream>
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Server::Server(void) {}

Server::Server(int port)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    _addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);

    std::cout << "Création du serveur IRC" << std::endl;
}

Server::~Server()
{
    std::for_each (_clients.begin(), _clients.end(), deleteClient);
    std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::start(void) {

    bind(_socket, (const struct sockaddr *)&_addr, sizeof(_addr));
    std::cout << _socket << std::endl;

    while (true)
    {
        listen(_socket, 5);
        std::cout << "listen !" << std::endl;


        _clients.push_back(new Client(_socket));

    }


}

void Server::stop(void) {
    this->run = 0;

    std::for_each (_clients.begin(), _clients.end(), closeClient);
    close(_socket);
}

/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */



/* ************************************************************************** */
/*                            NON MEMBER FUNCTIONS                            */
/* ************************************************************************** */

// ------------------------------- Stop utils ------------------------------- //
void    closeClient(Client* client) {
    close(client->getSocket());
}

// ---------------------------- Destructor utils ---------------------------- //
void    deleteClient(Client* client) {
    delete client;
}