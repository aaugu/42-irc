/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:59:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/01 13:03:39 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/signal.hpp"

bool sig::stopServer = false;

void sig::signalHandler( int signum )
{
	std::cout << "[Sig Handler] Caught signal " << signum << std::endl;
	if ( signum == SIGINT )
	{
		sig::stopServer = true;
	}
}
