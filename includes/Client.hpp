/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:37:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/12 15:43:48 by lvogt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string.h>
# include <vector>

# include "Server.hpp"

struct s_message {
    std::string                 _fullStr;
    std::string                 _command;
    std::vector<std::string>    _paramsSplit;
    std::string                 _params;
};

class Server;

class Client
{
    private:
		int			_sockfd;
		std::string _nickname;
		s_message	_message;
        bool        _isOp;

        void nickFunction(Server *s, std::vector<std::string> &data);
        void setOperatorState(Server *s, std::vector<std::string> args);

    public:
   		// Constructor and destructor
		Client(int sockfd);
		~Client(void);

		// Accessors
		int			getFd(void);
		std::string getNickname(void);

		// Gestion Input + Parsing + Execution
		void 		splitMessage(std::string buff);
        void		parseMessage(std::string buff);
		void		exeCommand(Server *s);
		void		saveMessage(std::string buff);
		void		send_to(std::string text) const;
};

#endif