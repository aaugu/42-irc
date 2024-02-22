/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:45:05 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/22 13:26:15 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <sys/socket.h>
# include <netinet/in.h>

class Client
{
	private:
		int 				_sockfd;
		struct sockaddr_in	_addr;
		socklen_t			_addrlen;
		
		Client(void);

	public:
		Client(int sockfd, const struct sockaddr_in& addr, socklen_t addr_len);
		~Client(void);

		// Accessors
		int	getSocket(void);
};

#endif