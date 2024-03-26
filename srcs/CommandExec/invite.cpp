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
    _client->sendMessage("Coucou\r\n");
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */