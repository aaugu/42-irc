#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                   INVITE                                   */
/* ************************************************************************** */

void    CommandExec::invite() {
    Client *itC = getptrClientByName();
    if (itC != nullptr)
    {
        _client->sendMessage("You have send invitation at " + itC->getNickname() + " to join #" + _msg->_paramsSplit[1] + " channel\r\n");
    } else {
        _client->sendMessage("Please enter a valid user\r\n");
    }
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */