/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:49:46 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/01 09:57:12 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLING_HPP
# define ERROR_HANDLING_HPP

# include <string>
# include <sstream>

template < typename T >
std::string errMessage(T className, int value, T message)
{
    std::stringstream   ss;
    ss << className;
	if (value != -1)
		ss << value << " ";
	ss << message;
    return (ss.str());
}

#endif
