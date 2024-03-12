#pragma once

# define MSG_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
# define MSG_FIRSTJOIN(nick) ("451 " + nick + " :\r\n")