#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// ERRORS
#define ERR_NOUSER      "ERROR : Invalid user, please enter a valide nickname\r\n"

/* ************************************************************************** */
/*                                   INVITE                                   */
/* ************************************************************************** */

void    CommandExec::invite() {
    if (getptrClientByName(_msg->_paramsSplit[0])) {

    } else {
        _client->sendMessage(ERR_NOUSER);
    }
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */