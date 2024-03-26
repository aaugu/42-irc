#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "SendMessages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

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