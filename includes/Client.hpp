/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:37:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/05 12:16:14 by aaugu            ###   ########.fr       */
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
		std::string	_username;
		s_message	_message;
        bool        _isOp;
		std::string	_address;
		bool		_passwordReceved;
        bool		_passwordChecked;
		bool		_welcomSended;

		void 						getUserCmdInfo();

    public:
   		// Constructor and destructor
		Client(int sockfd, std::string address);
		~Client(void);

		// Gestion Input + Parsing + Execution
		void 		splitMessage(std::string buff);
        void		parseMessage(std::string buff);
		void		eraseFullstr(void);
		void		exeCommand(Server* server);
		void		saveMessage(std::string buff);

		// Accessors
		int			getFd(void);
		std::string getNickname(void);
		std::string	getUsername(void);
		std::string	getAddress(void);
        bool        getOperatorState();
        bool        isPasswordReceved();
        bool        isPasswordChecked();
        bool        isWelcomSended();

        void        setOperatorState(bool value);
        void        setWelcomSended(bool welcomSended);
        void        setPasswordReceved(bool passwordReceved);
        void        setPasswordChecked(bool passwordChecked);
        void        setNickname(std::string nickname);

		// Send Message
		template < typename T >
		void	sendMessage(T message)
		{
			std::stringstream	ss;
			ss << message;
			std::string			msg = ss.str();

			if (send(_sockfd, msg.c_str(), msg.size(), 0) < 0)
				printErrMessage(errMessage("Could not send message to", _sockfd, strerror(errno)));
		}
};
#endif
