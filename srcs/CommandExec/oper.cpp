#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void CommandExec::oper(Server *server, Client *client, std::vector<std::string> args) {
    std::vector<Client>::iterator itC = server->getClientByNickname(args[0]);
    if (args[1] == server->getOpPass()) {
        if (!client->getOperatorState()) {
            if (client->getNickname() == args[0]) {
                client->sendMessage("You are now operator of this server\r\n");
                // TODO create "set op state"
                itC->setOperatorState(true);;
                return;
            }
            else {
                client->sendMessage("ERROR : You need to be operator to change permission of other user\r\n");
                return;
            }
        }
        else {

            if (itC->getFd() == 0 || itC->getFd() == 49) { // fd == 49 error if no other user connected in wsl (TODO tester sous mac)
                client->sendMessage("ERROR : Invalid user, please enter a valide nickname\r\n");
                return;
            }

            if (!itC->getOperatorState()) {
                client->sendMessage( "You have promoted " + itC->getNickname() + " Operator\r\n");
                sendMessageTo("You have been promoted Operator by " + client->getNickname() + "\r\n", itC->getFd());
                // TODO create "set op state"
                itC->setOperatorState(true);
                return;
            }
            else {
                if (client->getNickname() == args[0]){
                    client->sendMessage("You are no longer operator\r\n");
                }else {
                    client->sendMessage( itC->getNickname() + " is no longer operator\r\n");
                    sendMessageTo(client->getNickname() + " have removed your permission\r\n", itC->getFd());
                }
                // TODO create "set op state"
                itC->setOperatorState(false);
                return;
            }
        }
    }
    client->sendMessage("ERROR : Invalid password");
}