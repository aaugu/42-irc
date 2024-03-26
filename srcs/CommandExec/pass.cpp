#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/SendMessages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// Messages
#define MSG_PASSOK "Password Accepted\r\n"

/* ************************************************************************** */
/*                                   PASS                                     */
/* ************************************************************************** */

void CommandExec::pass() {
    _client->setPasswordReceved(true);
    if (_msg->_params.compare(_server->get_password()) == 0 && !_client->isPasswordChecked()) {
        _client->setPasswordChecked(true);
        _client->sendMessage(MSG_PASSOK);
    }
    check_if_pass();
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */

