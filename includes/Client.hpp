/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:45:05 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/20 14:04:09 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <sys/socket.h>
# include <netinet/in.h>

class Client
{
	private:
		int 				_socket;
		struct sockaddr_in	_addr;
		socklen_t			_size;
		
		Client(void);

	public:
		Client(int servSocket);
		~Client(void);

		// Accessors
		int	getSocket(void);
};

#endif