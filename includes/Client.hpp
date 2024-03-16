/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:37:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/15 14:52:35 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>
# include <vector>
# include <poll.h>

struct s_message {
    std::string fullStr;
    std::string command;
    std::vector<std::string> _paramsSplit;
    std::string _params;
};

class Server;
class Channel;

class Client
{
    private:
		int			_sockfd;
		std::string	_nickname;
		s_message	_message;
		bool		_passwordReceved;
		bool		_passwordChecked;
		bool		_welcomSended;
		Channel*	_currentChannel;

		void		command_pass(Server &server, std::vector<pollfd>::iterator pollfd);
		void		check_if_pass(Server &server, std::vector<pollfd>::iterator pollfd);

		std::vector<std::string>	split(std::string value);
		void 						splitMessage(std::string buff);
		bool						checkUseNickname(Server *s, std::string &nickname);

    public:
   		// Constructor and destructor
		Client(int sockfd);
		~Client(void);

		// Class function
		void setData(Server *s, std::string &buffer);

		// Gestion Input + Parsing + Execution
        void		parseMessage(std::string buff);
		void		exeCommand(Server* server, std::vector<pollfd>::iterator pollfd);
		void		saveMessage(std::string buff);
		// void		send_to(std::string text) const;

		// Accessors
		int			getFd(void);
		std::string getNickname(void);
		// void		setFd(int value);
		void		setNickname(std::string value);
		void		setCurrentChannel(Channel* currentChannel);
};

#endif