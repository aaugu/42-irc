/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:41:39 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/20 18:21:33 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <string>
# include <map>

class Client;
class Server;

class Channel
{
    private:
		std::string				_name;
		std::string				_password;
		std::string				_topic;
		int						_userLimit;

		std::map<Client*, bool>	_users;

		bool	_modeI;	// invite-only
		bool	_modeT;	// topic command only for operator
		bool	_modeK;	// password
		bool	_modeL;	// userlimit

    public:
   		// Constructor and destructor
		Channel(std::string name, Client* user);
		~Channel(void);

		// Modifiers
		void	addUser(Client* user, bool isOperator);
		void	removeUser(Client* user, Server* server);

		// Checks
		bool	isOperator(Client* user);
		bool	isPasswordValid(std::string password);
		bool	isUserPresent(Client* client);

		// Send
		void	sendMessageToUsers(std::string message);
		void	sendMessageToUsersExceptSender(Client* sender, std::string message);

		// Accessors
		std::string				getName(void);
		std::string				getPassword(void);
		int						getUserLimit(void);
		std::map<Client*, bool>	getUsers(void);
		bool					getModeI(void);
		bool					getModeT(void);
		bool					getModeK(void);
		bool					getModeL(void);

		std::string				getAllUsersList(void);


};

#endif
