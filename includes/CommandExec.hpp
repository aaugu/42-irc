/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExec.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:57:56 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/28 16:42:48 by aaugu            ###   ########.fr       */
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
#define RPL_PRIVMSG(client, target, message) (":" + USER(client) + " PRIVMSG " + target + " " + message + "\r\n")
#define RPL_PART(client, channel, message) (":" + USER(client) + " PART " + channel + " " + message + "\r\n")
#define RPL_YOUREOPER(channel, client) ("381 " + client + " :You are now an channel " + channel + " operator\r\n")
#define RPL_CHANNELMODEIS(address, client, channel, mode) (":" + address + " 324 " + client + " " + channel + " " + mode + "\r\n")

#define ERR_NOSUCHCHANNEL(address, client, channel) (":" + address + " 403 " + client + " " + channel + " :Channel name is invalid, or does not exist\r\n")
#define ERR_NOSUCHNICK(address, client)  (":" + address + " 401 " + client + " :Nickname is invalid, or does not exist\r\n")
#define ERR_USERNOTINCHANNEL(client, channel) ("441 " + client + " " + channel + " : They aren’t on that channel\r\n")
#define ERR_NOTONCHANNEL(address, client, channel) (":" + address + " 442 " + client + " " + channel + " :You're not on that channel\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) ("482 " + channel + " :You're not channel operator\r\n")
#define ERR_UNKNOWNMODE(servername, nick, mode) (":" + servername + " 472 " + nick + " " + mode + " :mode unknown \r\n")
#define ERR_NEEDMOREPARAMS(address, client, command) (":" + address + " 461 " + client + " " + command + " :Not enough parameters given\r\n")

struct s_flag
{
    char flag;
    int sign; // 1 is +, 2 is -
    bool isValid;
    s_flag *next;
    s_flag *prev;
    std::string option;
};

class CommandExec
{
    private:
		Server*		_server;
		Client*		_client;
		s_message*	_msg;

		// PRIVATE UTILS
		bool		invalidNbParams(int nbParams, int minNbParams, int maxNbParams);
        Client* 	getptrClientByName(std::string nickname);
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

		// MODE
		s_flag	*parserOption(std::string flags);
		s_flag	*updateStruct(s_flag *newFlag, int sign, bool isValid);
		bool	checkParsing(s_flag *parsed, std::vector<std::string> options);
		void	interpretMode(s_flag *parsed, std::vector<std::string> options, Channel &channel);

		// TOPIC
		void	sendTopic(Channel* channel);
		void	modifyTopic(Channel* channel);
		// etc.

    public:
   		// Constructor and destructor
		CommandExec(Server* server, Client* client, s_message* msg);
		~CommandExec(void);

		// Commands
		void	    join(void);
		void		mode(void);
        void        oper(void);
        void        kill(void);
        void        nick(void);
        void        ping(void);
        void        quit(void);
        void        pass(void);
        void        invite(void);
		void	    privmsg(void);
		void	    part(void);
		void		topic(void);
		// etc.

        // PUBLIC UTILS
        void    check_if_pass();

};

#endif
