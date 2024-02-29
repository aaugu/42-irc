/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:29 by aaugu             #+#    #+#             */
/*   Updated: 2024/02/22 12:45:00 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error_handling.hpp"

std::string errMessage(std::string className, char * message)
{
    std::stringstream   ss;
    ss << className << message;
    return (ss.str());
}