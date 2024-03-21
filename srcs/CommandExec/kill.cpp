#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                   KILL                                     */
/* ************************************************************************** */

//void    CommandExec::killClient(std::vector<std::string> args) {
//    if (!_isOp) {
//        sendMessage("ERROR : You need to be operator to kill a client\r\n");
//        return;
//    }
//
//    if (s->checkClientPresence(args[0])) {
//        std::vector<Client>::iterator itC = s->getClientByNickname(args[0]);
//        if (args[1] == ":") {
//            args[1] += "unknown reason";
//        }
//        sendMessageTo("You have been kicked by " + _nickname  + " " + args[1] + "\r\n", itC->getFd());
//        s->disconnectClient(&(*itC));
//        sendMessage(itC->getNickname() + " has been disconnected\r\n");
//    } else {
//        sendMessage("ERROR : it's not a valid user\r\n");
//    }
//}

/* ************************************************************************** */
/*                               SUB FUNCTIONS                                */
/* ************************************************************************** */
