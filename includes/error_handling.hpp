/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:49:46 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/22 12:44:50 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HANDLING_HPP
# define ERROR_HANDLING_HPP

# include <string>
# include <errno.h>
# include <cstring>
# include <sstream>

std::string errMessage(std::string className, char * message);

#endif