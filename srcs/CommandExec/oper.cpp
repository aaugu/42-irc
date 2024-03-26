#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

// MESSAGES

#define MSG_NOWOPER                         "You are now operator of this server\r\n"
#define MSG_REMOVEOPER                      "You are no longer operator\r\n"
#define MSG_REMOVEOTHEROPER(nickname)       (nickname + " is no longer operator")
#define MSG_REMOVEOPERBY(nickname)          (nickname + " have removed your permission\r\n")
#define MSG_PROMOTOTHER(nickname)           ("You have promoted " + nickname + " Operator\r\n")
#define MSG_PROMOTBY(nickname)              ("You have been promoted Operator by " + nickname + "\r\n")

//ERRORS

#define ERR_NOPERM      "ERROR : You need to be operator to change permission of other user\r\n"
#define ERR_NOUSER      "ERROR : Invalid user, please enter a valide nickname\r\n"
#define ERR_WRONGPASS   "ERROR : Invalid password\r\n"

/* ************************************************************************** */
/*                                   OPER                                     */
/* ************************************************************************** */

void CommandExec::oper() {
    std::vector<std::string> nicknameList = _server->getNicknameList();
    if (getptrClientByName(_msg->_paramsSplit[0]) == nullptr) {
        if (_client->getOperatorState())
            _client->sendMessage(ERR_NOUSER);
        else
            _client->sendMessage(ERR_NOPERM);
        return;
    }

    std::vector<Client>::iterator itC = _server->getClientByNickname(_msg->_paramsSplit[0]);

    if (_msg->_paramsSplit[1] == _server->getOpPass()) {
        if (!_client->getOperatorState()) {
            if (_client->getNickname() == _msg->_paramsSplit[0]) {
                _client->sendMessage(MSG_NOWOPER);
                itC->setOperatorState(true);;
                return;
            }
            else {
                _client->sendMessage(ERR_NOPERM);
                return;
            }
        }
        else {
            if (!itC->getOperatorState()) {
                _client->sendMessage(MSG_PROMOTOTHER(itC->getNickname()));
                sendMessageTo(MSG_PROMOTBY(_client->getNickname()), itC->getFd());
                itC->setOperatorState(true);
                return;
            }
            else {
                if (_client->getNickname() == _msg->_paramsSplit[0]){
                    _client->sendMessage(MSG_REMOVEOPER);
                }else {
                    _client->sendMessage(MSG_REMOVEOTHEROPER(itC->getNickname()));
                    sendMessageTo(MSG_REMOVEOPERBY(_client->getNickname()), itC->getFd());
                }
                itC->setOperatorState(false);
                return;
            }
        }
    }
    _client->sendMessage(ERR_WRONGPASS);
}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */