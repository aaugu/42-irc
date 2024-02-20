/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:44:58 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/20 14:38:57 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/Client.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Client::Client(void) {}

Client::Client(int servSocket)
{
	_size = sizeof(_addr);
    _socket = accept(servSocket, (struct sockaddr *)&_addr, &_size);

	if (_socket < 0)
        std::cerr << "Error accepting connection" << std::endl;
	else
        send(_socket, "hello\n", 5, 0);
}

Client::~Client() {}

/* ************************************************************************** */
/*                                 ACCESSORS                                  */
/* ************************************************************************** */

int	Client::getSocket(void) {
	return (this->_socket);
}