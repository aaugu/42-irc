/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:44:58 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/22 13:26:05 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/Client.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Client::Client(void) {}

Client::Client(int sockfd, const struct sockaddr_in& addr, socklen_t addr_len)
{
	_addrlen = addr_len;
    _sockfd = sockfd;
	_addr.sin_family = addr.sin_family;
	_addr.sin_port = addr.sin_port;
	_addr.sin_addr.s_addr = addr.sin_addr.s_addr;

}

Client::~Client() {}

/* ************************************************************************** */
/*                                 ACCESSORS                                  */
/* ************************************************************************** */

int	Client::getSocket(void) {
	return (_sockfd);
}