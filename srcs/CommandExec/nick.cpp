#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/SendMessages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// MESSAGE
#define MSG_SETNICKNAME(nickname) ("Your nickname was set to " + _msg->_paramsSplit[0] + "\r\n")

// REPLY
#define RPL_CHANGENICKNAME(nickname) (": NICK " + nickname + "\r\n")

// ERRORS
#define ERR_INVALIDCHAR "Nickname can't start with '#', we have remove it for you\r\n"
#define ERR_ALLREADYUSEDNICKNAME(nickname) ("Nickname already used, Nickname changed to " + nickname + "\r\n")

/* ************************************************************************** */
/*                                   NICK                                     */
/* ************************************************************************** */

std::string CommandExec::nick() {
    bool dupe = false;

    if (_msg->_paramsSplit[0][0] == '#') {
        _msg->_paramsSplit[0].erase(0, 1);
        _client->sendMessage(ERR_INVALIDCHAR);
    }

    while (getptrClientByName() != nullptr) {
        _msg->_paramsSplit[0] += '_';
        dupe = true;
    }
    if (dupe) {
        _client->sendMessage(RPL_CHANGENICKNAME(_msg->_paramsSplit[0]));
        _client->sendMessage(ERR_ALLREADYUSEDNICKNAME(_msg->_paramsSplit[0]));
    }

    if (_client->isPasswordReceved() && _client->isPasswordChecked() && !_client->isWelcomSended()){
        _client->sendMessage(RPL_WELCOME(_msg->_paramsSplit[0], "_user", "_hostName"));
        _client->setWelcomSended(true);
    } else if (_client->isPasswordReceved() && _client->isPasswordChecked() && _client->isWelcomSended()) {
        _client->sendMessage(MSG_SETNICKNAME(_msg->_paramsSplit[0]));
    }

    return _msg->_paramsSplit[0];
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */