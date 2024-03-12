/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvogt <lvogt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:59:30 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/12 16:09:12 by lvogt            ###   ########.fr       */
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
