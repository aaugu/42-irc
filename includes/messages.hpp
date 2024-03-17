/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 11:27:49 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/17 17:39:51 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# include <string>
# include <iostream>
# include <sstream>
# include <errno.h>
# include <string.h>
# include <sys/socket.h>

# define RED  "\e[31m"
# define END  "\e[0m"

template < typename T >
std::string errMessage(std::string message, int value, T errnoMessage)
{
    std::stringstream   ss;
    ss << message << " ";
	if (value != -1)
		ss << value << " : ";
	else
		ss << ": ";
	ss << errnoMessage;
    return (ss.str());
}

template < typename T >
void	printErrMessage(T errMessage)
{
	std::cerr << RED << errMessage << END << std::endl;
}

template < typename T >
void	sendMessage(T message, int sockfd)
{
	std::stringstream	ss;
	ss << message << std::endl;
	std::string			msg = ss.str();

	if (send(sockfd, msg.c_str(), msg.size(), 0) < 0)
		printErrMessage(errMessage("Could not send message to", sockfd, strerror(errno)));
}

#endif
