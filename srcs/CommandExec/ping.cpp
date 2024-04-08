#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

#define ERR_NOORIGIN(command) ("409 " + command + " :No origin specified\r\n")
#define PONG(server) ("PONG " + server + "\r\n")

/* ************************************************************************** */
/*                                   PING                                     */
/* ************************************************************************** */

void CommandExec::ping(void) {
    if (_msg->_params.empty()) {
        _client->sendMessage(ERR_NOORIGIN(_msg->_command));
        return;
    }
    else
        _client->sendMessage(PONG(_msg->_params));
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */