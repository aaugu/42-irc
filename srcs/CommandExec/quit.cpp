#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                   QUIT                                     */
/* ************************************************************************** */

void CommandExec::quit() {
    //TODO envoyer un message "Machin" + _message._params
    // à tout les utilisateurs des channels de Machin
    _server->disconnectClient(_client);
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */