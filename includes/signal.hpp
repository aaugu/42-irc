/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:01:55 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/01 13:03:30 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

# include <csignal>

namespace sig
{
	extern bool stopServer;
	void signalHandler( int signum );
}

#endif
