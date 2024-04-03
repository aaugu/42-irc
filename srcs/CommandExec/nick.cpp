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
#define RPL_CHANGENICKNAME(old, nickname) (":" + old + " NICK " + nickname + "\r\n")

// ERRORS
#define ERR_INVALIDCHAR                     "Nickname can't start with '#', we have remove it for you\r\n"
#define ERR_ALLREADYUSEDNICKNAME(nickname)  ("Nickname already used, Nickname changed to " + nickname + "\r\n")
#define ERR_SAMENICKNAME(nickname)          ("Your have already " + nickname + " has nickname\r\n")

/* ************************************************************************** */
/*                                   NICK                                     */
/* ************************************************************************** */

void CommandExec::nick() {
    bool dupe = false;
    bool done = false;
    std::string old = _client->getNickname();

    if (_client->getNickname() == _msg->_paramsSplit[0]) {
        _client->sendMessage(ERR_SAMENICKNAME(_client->getNickname()));
        return;
    }

    if (_msg->_paramsSplit[0][0] == '#') {
        _msg->_paramsSplit[0].erase(0, 1);
        _client->sendMessage(ERR_INVALIDCHAR);
    }

    while (getptrClientByName(_msg->_paramsSplit[0]) != NULL) {
        _msg->_paramsSplit[0] += '_';
        dupe = true;
    }
    if (dupe) {
        _client->sendMessage(ERR_ALLREADYUSEDNICKNAME(_msg->_paramsSplit[0]));
    }

    if (_client->isPasswordReceved() && _client->isPasswordChecked() && !_client->isWelcomSended()){
        _client->sendMessage(RPL_WELCOME(_msg->_paramsSplit[0], "_user", "_hostName"));
        _client->setWelcomSended(true);
    } else if (_client->isPasswordReceved() && _client->isPasswordChecked() && _client->isWelcomSended()) {
        _client->sendMessage(RPL_CHANGENICKNAME(old, _msg->_paramsSplit[0]));
        _client->sendMessage(MSG_SETNICKNAME(_msg->_paramsSplit[0]));
    }

    _client->setNickname(_msg->_paramsSplit[0]);

    std::vector<Channel> chan = _server->getChannels();

    std::vector<Channel>::iterator itChan;

    for (itChan = chan.begin(); itChan < chan.end(); itChan++) {
        if (!done) {
            std::cout << "DEBUG nickname : " << itChan->getName() << std::endl;
            itChan->sendMessageToUsersExceptSender(_client, RPL_CHANGENICKNAME(old, _msg->_paramsSplit[0]));
            done = true;
        }
    }
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */