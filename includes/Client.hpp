/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:37:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/22 15:08:14 by lvogt            ###   ########.fr       */
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
    std::string                 _fullStr;
    std::string                 _command;
    std::vector<std::string>    _paramsSplit;
    std::string                 _params;
};

class Server;
class Channel;

class Client
{
    private:
		int			_sockfd;
		std::string _nickname;
		s_message	_message;
        bool        _isOp;
		std::string	_address;
		bool		_passwordReceved;
		bool		_passwordChecked;
		bool		_welcomSended;
		Channel*	_currentChannel;

		void		command_pass(Server &server);
		void		check_if_pass(Server &server);
		void		command_ping(void);
		void		command_quit(Server &server);

		void    nickFunction(Server *s, std::vector<std::string> &data);
        void    setOperatorState(Server *s, std::vector<std::string> args);
        void    killClient(Server *s, std::vector<std::string> args);

		bool						checkUseNickname(Server *s, std::string &nickname);
		std::vector<std::string>	split(std::string value);

    public:
   		// Constructor and destructor
		Client(int sockfd, std::string address);
		~Client(void);

		// Class function
		void setData(Server *s, std::string &buffer);

		// Gestion Input + Parsing + Execution
		void 		splitMessage(std::string buff);
        void		parseMessage(std::string buff);
		void		eraseFullstr(void);
		void		exeCommand(Server* server);
		void		saveMessage(std::string buff);

		// Accessors
		int			getFd(void);
		std::string getNickname(void);
		std::string	getAddress(void);
		void		setNickname(std::string value);
		void		setisInAChannel(bool);
		void		setCurrentChannel(Channel* currentChannel);

		// Send Message
		template < typename T >
		void	sendMessage(T message)
		{
			std::stringstream	ss;
			ss << message;
			std::string			msg = ss.str();

			if (send(_sockfd, msg.c_str(), msg.size(), 0) < 0)
				printErrMessage(errMessage("Could not seasdfnd message to", _sockfd, strerror(errno)));
		}
};
#endif
