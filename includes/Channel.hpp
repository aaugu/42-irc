/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:41:39 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/15 00:18:36 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <vector>
# include <string>
# include "Client.hpp"


class Channel
{
    private:
		std::string				_name;
		std::string				_password;
		Client*					_operator;
		std::vector<Client*>	_clients;
		bool					_hasOperator;
		bool					_hasPassword;


    public:
   		// Constructor and destructor
		Channel(std::string name, Client* client);
		~Channel(void);
};

#endif
