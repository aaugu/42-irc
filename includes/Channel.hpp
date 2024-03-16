/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:41:39 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/15 17:10:37 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <string>
class Client;

class Channel
{
    private:
		std::string				_name;
		bool					_operatorConnected;
		std::vector<Client*>	_clients;
		// std::string				_password;


    public:
   		// Constructor and destructor
		Channel(void) {}
		Channel(std::string name, Client* client);
		~Channel(void);

		void	addClient(Client* client);
		void	removeClient(Client* client);

		void	informClientArrival(Client* client);
		void	welcomeMessage(Client* client);

		// Accessors
		std::string				getName(void);
		bool					getOperatorConnected(void);
		std::vector<Client*>	getClients(void);
};

#endif
