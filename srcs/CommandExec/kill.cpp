#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// MESSAGES
#define MSG_KICKMSG(nickname, reason)   ("You have been kicked by " + nickname  + " " + reason + "\r\n")
#define MSG_OTHERKILLMSG(nickname)      (nickname + " has been disconnected\r\n")

// ERRORS
#define ERR_NOPERM          "ERROR : You need to be operator to kill a client\r\n"
#define ERR_NOUSER          "ERROR : it's not a valid user\r\n"
#define ERR_USERSESSIONKILL "ERROR : You can't kill your session\r\n"

/* ************************************************************************** */
/*                                   KILL                                     */
/* ************************************************************************** */

void    CommandExec::kill() {
    std::string reason;
    if (!_client->getOperatorState()) {
        _client->sendMessage(ERR_NOPERM);
        return;
    }

    if (_client->getNickname() == _msg->_paramsSplit[0]) {
        _client->sendMessage(ERR_USERSESSIONKILL);
        return;
    }

    if (getptrClientByName(_msg->_paramsSplit[0]) == NULL) {
        _client->sendMessage(ERR_NOUSER);
        return;
    }

    std::vector<Client>::iterator itC = _server->getClientByNickname(_msg->_paramsSplit[0]);
    if (_msg->_paramsSplit[1] == ":") {
        _msg->_paramsSplit[1] += "unknown reason";
    } else {
        std::vector<std::string>::iterator it;
        for (it = _msg->_paramsSplit.begin() + 1; it != _msg->_paramsSplit.end(); ++it) {
            reason += *it;
            reason += " ";
        }
    }
    sendMessageTo(MSG_KICKMSG(_client->getNickname(), reason), itC->getFd());
    _client->sendMessage(MSG_OTHERKILLMSG(itC->getNickname()));
    _server->disconnectClient(&(*itC));
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */