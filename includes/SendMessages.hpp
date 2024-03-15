#pragma once

# define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
# define ERR_NOTREGISTERED(nick) ("451 " + nick + " :\r\n")
# define ERR_PASSWDMISMATCH "464 __ :Password incorrect\r\n"
# define ERR_PASSWDMISS "464 __ :Password missing\r\n"