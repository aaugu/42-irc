/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExec.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:57:56 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/20 18:54:58 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDEXEC_HPP
#define COMMANDEXEC_HPP

# include <string>

class Server;
class Client;
class Channel;
struct s_message;
												// shoud be username
#define USER(user) (user->getNickname() + "!" + user->getNickname() + "@" + user->getAddress())
#define ERR_NOSUCHCHANNEL(address, client, channel) (":" + address + " 403 " + client + " " + channel + ":Channel name is invalid, or does not exist\r\n")
#define ERR_NOSUCHNICK(address, client)  (":" + address + " 401 " + client + " " + ":Nickname is invalid, or does not exist\r\n")
#define RPL_PRIVMSG(client, target, message) (":" + USER(client) + " PRIVMSG " + target + " " + message + "\r\n")

class CommandExec
{
    private:
		Server*		_server;
		Client*		_client;
		s_message*	_msg;

		// UTILS
		bool		minNbParams(int nbParams, int minNbParams);
		std::string	getFullMessage(void);

		// JOIN
		void	checkChannelName(std::string& name);
		void	createChannel(std::string name);
		void	joinChannel(Channel& channel);

		// PRIVMSG
		void	sendMessageToChannel(std::string target, std::string message);
		void	sendPrivateMessage(std::string target, std::string message);

		// etc.

    public:
   		// Constructor and destructor
		CommandExec(Server* server, Client* client, s_message* msg);
		~CommandExec(void);

		// Commands
		void	join(void);
		void	privmsg(void);
		void	part(void);
		// etc.

};

#endif
