/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandling.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:49:46 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/01 10:22:40 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORHANDLING_HPP
# define ERRORHANDLING_HPP

# include <string>
# include <sstream>

# define RED  "\e[31m"
# define END  "\e[0m"

template < typename T >
std::string errMessage(std::string className, int value, T message)
{
    std::stringstream   ss;
    ss << className << " ";
	if (value != -1)
		ss << value << " : ";
	else
		ss << ": ";
	ss << message;
    return (ss.str());
}

template < typename T >
void	printErrMessage(T errMessage)
{
	std::cerr << RED << errMessage << END << std::endl;
}

#endif
