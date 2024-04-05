#include <string>
#include <algorithm>

#include "../includes/CommandExec.hpp"
#include "../includes/messages.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void	CommandExec::mode(void){
    std::stringstream ss(_msg->_params);
    std::string word;
    std::string channel;
    std::string flags = "\0";
    std::vector<std::string> optionsArray;
    int count = 0;
    Channel *channel_ptr = _server->getptrChannelByName(_msg->_paramsSplit[0]);

    while (ss >> word) {
        if (channel_ptr != NULL){
            if (count == 0 && channel_ptr->isUserPresent(_client) == true && word != channel_ptr->getName())
                flags = word;
            else
                channel = word;
        }
        if (count == 1)
            flags += word;
        else if (count != 0)
            optionsArray.push_back(word);
        count++;
    }
    std::string test = _client->getNickname() + " +i";
    if (_msg->_params == test)
        return;
    if (channel_ptr == NULL) {
        _client->sendMessage(ERR_NOSUCHCHANNEL(_client->getAddress(), _client->getNickname(), _msg->_paramsSplit[0]));
        return;
    }
    else
    {
        if (channel_ptr->isUserPresent(_client) == false) {
            _client->sendMessage(ERR_NOTONCHANNEL(_client->getAddress(), _client->getNickname(), channel_ptr->getName()));
            return;
        }
        else {
            if (!channel_ptr->isOperator(_client) && flags != "\0") {
                _client->sendMessage(ERR_CHANOPRIVSNEEDED(channel_ptr->getName()));
                return;
            }
        }
    }
    if (flags.empty())
    {
        std::string modes = "+";
        if (channel_ptr->getModeI())
            modes += "i";
        if (channel_ptr->getModeT())
            modes += "t";
        if (channel_ptr->getModeL())
            modes += "l";
        if (channel_ptr->getModeK())  
            modes += "k";
        if (modes == "+")
            modes = "\0";
        _client->sendMessage(RPL_CHANNELMODEIS(_client->getAddress(), _client->getNickname(), channel_ptr->getName(), modes));
        return;
    }
    s_flag *parsed;
    if (flags[0] == '+' || flags[0] == '-') {
        parsed = parserOption(flags);
        if (checkParsing(parsed, optionsArray))
            interpretMode(parsed, optionsArray, (*channel_ptr));
        s_flag *currentFlag = parsed;
        while (currentFlag != NULL){
            s_flag *nextFlag = currentFlag->next;
            delete currentFlag;
            currentFlag = nextFlag; 
        }
    }
    return;
}

bool CommandExec::checkParsing(s_flag *parsed, std::vector<std::string> options)
{
    int i = 0;
    while (parsed != NULL)
    {
        if (parsed->flag == 'i' || parsed->flag == 't')
        {
            if (parsed->sign != 1 && parsed->sign != 2)
                parsed->isValid = false;
        }
        else if ((parsed->flag == 'k' || parsed->flag == 'l' || parsed->flag == 'o') && (parsed->sign == 1 || parsed->sign == 2))
        {
            if (parsed->sign == 1)
            {
                if (options.empty() || i >= (int)options.size())
                    return (false);
                else
                    parsed->option = options[i];
                i++;
            }
        }
        else
            return (false);
        parsed = parsed->next;
    }
    return (true);
}

void CommandExec::interpretMode(s_flag *parsed, std::vector<std::string> options, Channel &channel)
{
    int i = 0;
    std::string toSendFlagsPos;
    std::string toSendFlagsNeg;
    std::string toSendOptions;
    while(parsed != NULL)
    {
        if (parsed->flag == 't' && parsed->sign == 1)
        {
            if (!channel.getModeT())
            {
                channel.setModeT(true);
                toSendFlagsPos += "t";
            }
        }
        else if (parsed->flag == 't' && parsed->sign == 2)
        {
            if (channel.getModeT())
            {
                channel.setModeT(false);
                toSendFlagsNeg += "t";
            }
        }
        else if (parsed->flag == 'i' && parsed->sign == 1)
        {
            if (!channel.getModeI())
            {
                channel.setModeI(true);
                toSendFlagsPos += "i";
            }
        }
        else if (parsed->flag == 'i' && parsed->sign == 2)
        {
            if (channel.getModeI())
            {
                channel.setModeI(false);
                channel.clearWhitelist();
                toSendFlagsNeg += "i";
            }
        }
        else if (parsed->flag == 'l' && parsed->sign == 1)
        {
            try
            {
                char *endptr;
                int limit = (int)std::strtol(options[i].c_str(),  &endptr, 10);
                if (limit < 0 || *endptr != '\0')
                    throw std::exception();
                channel.setModeL(true);
                channel.setUserLimit(limit);
                std::string flags = "+l " + options[i];
                toSendFlagsPos += "l";
                toSendOptions += options[i] + " ";
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            i++;
        }
        else if (parsed->flag == 'l' && parsed->sign == 2)
        {
            if (channel.getModeL())
            {
                channel.setModeL(false);
                toSendFlagsNeg += "l";
            }
        }
        else if (parsed->flag == 'k' && parsed->sign == 1)
        {
           
            channel.setModeK(true);
            channel.setPassword(options[i]);
            std::string flags = "+k " + options[i];
            toSendFlagsPos += "k";
            toSendOptions += options[i] + " ";
            i++;
        }
        else if (parsed->flag == 'k' && parsed->sign == 2)
        {
            if (channel.getModeK())
            {
                channel.setModeK(false);
                toSendFlagsNeg += "k";
            }
            i++;
        }
        else if (parsed->flag == 'o' && parsed->sign == 1)
        {
            std::string nickname = options[i];
            std::string flags = "+o " + options[i];
            Client *target = channel.getMapptrClientByName(nickname);
            if (channel.isUserPresent(target) == true)
            {
                channel.setOperator(target, true);
                target->sendMessage(RPL_YOUREOPER(channel.getName(),nickname));
                toSendFlagsPos += "o";
                toSendOptions += options[i] + " ";
            }
            else
                _client->sendMessage(ERR_USERNOTINCHANNEL(nickname, channel.getName()));
            i++;
        }
        else if (parsed->flag == 'o' && parsed->sign == 2)
        {

            std::string nickname = options[i];
            std::string flags = "-o " + options[i];
            Client *target = channel.getMapptrClientByName(nickname);
            if (channel.isUserPresent(target) == true)
            {
                channel.setOperator(target, false);
                target->sendMessage(RPL_YOUREOPER(channel.getName(),nickname));
                toSendFlagsNeg += "o";
                toSendOptions += options[i] + " ";
            }
            else
                _client->sendMessage(ERR_USERNOTINCHANNEL(nickname, channel.getName()));
            i++;
        }
        else
            _client->sendMessage(ERR_UNKNOWNMODE(_client->getAddress(), _client->getNickname(), parsed->flag).c_str());
        parsed = parsed->next;
    }
    if (toSendFlagsPos.size() > 0)
        toSendFlagsPos = "+" + toSendFlagsPos;
    if (toSendFlagsNeg.size() > 0)
        toSendFlagsNeg = "-" + toSendFlagsNeg;
    if (toSendOptions.size() > 0)
        toSendOptions = " " + toSendOptions;
    std::string finalString = toSendFlagsPos + toSendFlagsNeg + toSendOptions;
    if (finalString.size() > 0)
        channel.sendMessageToUsers(RPL_CHANNELMODESET(_client, channel.getName(), finalString));
    return;
}

s_flag  *CommandExec::parserOption(std::string flags)
{
    int i = 0;
    s_flag* head = NULL;
    s_flag *currentFlag = NULL;
    int parsedSign = 0;
    while (flags[i] != '\0')
    {
        if (flags[i] == '+')
            parsedSign = 1;
        else if (flags[i] == '-')
            parsedSign = 2;
        else
        {
            s_flag *newFlag = new s_flag;
            if (head == NULL)
            {
                head = newFlag;
                head->prev = NULL;
                currentFlag = newFlag;
                currentFlag->prev = NULL;
            }
            else
            {
                currentFlag->next = newFlag;
                newFlag->prev = currentFlag;
                currentFlag = newFlag;
            }
            newFlag->flag = flags[i];
            if (parsedSign != 0)
                updateStruct(newFlag, parsedSign, true);
            else if (currentFlag != NULL && currentFlag->prev != NULL)
                updateStruct(newFlag, currentFlag->prev->sign, true);
            else
                updateStruct(newFlag, 0, false);
            parsedSign = 0;
        }
        i++; 
    }
    return (head);
}

s_flag *CommandExec::updateStruct(s_flag *newFlag, int sign, bool isValid)
{
    newFlag->sign = sign;
    newFlag->isValid = isValid;
    newFlag->next = NULL;
    return (newFlag);
}