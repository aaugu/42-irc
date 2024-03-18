/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:37:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/15 10:42:28 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string.h>
# include <vector>

# include "Server.hpp"

struct s_message {
    std::string _fullStr;
    std::string _command;
    std::vector<std::string> _paramsSplit;
    std::string _params;
};

class Server;

class Client
{
    private:
		int			_sockfd;
		std::string	_nickname;
		s_message	_message;
		bool		_passwordReceved;
		bool		_passwordChecked;
		bool		_welcomSended;

		// Client(void);

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

		// Gestion Input + Parsing + Execution
		void 		splitMessage(std::string buff);
        void		parseMessage(std::string buff);
		void		exeCommand(Server &server, std::vector<pollfd>::iterator pollfd);
		void		saveMessage(std::string buff);
		void		send_to(std::string text) const;
		

		// command

		void		command_pass(Server &server, std::vector<pollfd>::iterator pollfd);
		void		check_if_pass(Server &server, std::vector<pollfd>::iterator pollfd);
};

#endif