/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:37:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/18 12:02:57 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>
# include <vector>
# include <sstream>
# include <poll.h>
# include <errno.h>
# include <string.h>

# include "../includes/messages.hpp"

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
		std::string	_address;
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
		Client(int sockfd, std::string address);
		~Client(void);

		// Class function
		void setData(Server *s, std::string &buffer);

		// Gestion Input + Parsing + Execution
        void		parseMessage(std::string buff);
		void		exeCommand(Server* server, std::vector<pollfd>::iterator pollfd);
		void		saveMessage(std::string buff);

		// Accessors
		int			getFd(void);
		std::string getNickname(void);
		std::string	getAddress(void);
		void		setNickname(std::string value);
		void		setCurrentChannel(Channel* currentChannel);

		// Send Message
		template < typename T >
		void	sendMessage(T message)
		{
			std::stringstream	ss;
			ss << message << std::endl;
			std::string			msg = ss.str();

			if (send(_sockfd, msg.c_str(), msg.size(), 0) < 0)
				printErrMessage(errMessage("Could not send message to", _sockfd, strerror(errno)));
		}
};

#endif