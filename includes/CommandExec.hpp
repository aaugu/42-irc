/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExec.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:57:56 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/17 18:56:56 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDEXEC_HPP
#define COMMANDEXEC_HPP

# include <string>

class Client;
class Server;
struct s_message;
class Channel;

class CommandExec
{
    private:
		Server*		_server;
		Client*		_client;
		s_message*	_msg;

		// PRIVATE UTILS
		bool	invalidNbParams(int nbParams, int minNbParams, int maxNbParams);
        Client* getptrClientByName();

		// JOIN
		void	checkChannelName(std::string& name);
		void	createChannel(std::string name);
		void	joinChannel(Channel& channel);

		// etc.

    public:
   		// Constructor and destructor
		CommandExec(Server* server, Client* client, s_message* msg);
		~CommandExec(void);

		// Commands
		void	    join();
        void        oper();
        void        kill();
        std::string nick();
        void        ping();
        void        quit();
        void        pass();
		// etc.

        // PUBLIC UTILS
        void    check_if_pass();

};

#endif
