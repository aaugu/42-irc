/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:37:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/12 11:56:54 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>
#include "Server.hpp"

class Server;

class Client
{
    private:
		int			_sockfd;
		std::string	_nickname;

    public:
   		// Constructor and destructor
		Client(int sockfd);
		~Client(void);

		// Class function
		void setData(Server *s, std::string &buffer);

		// Accessors
		int			getFd(void);
		std::string getNickname(void);
		// void		setFd(int value);
		void		setNickname(std::string value);
};

#endif