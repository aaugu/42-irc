#include "Client.hpp"

std::string t(const std::string& input) {
    std::string result;
    for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
        char c = *it;
        switch (c) {
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            // Ajoutez d'autres caractères spéciaux si nécessaire
            default:
                result += c;
                break;
        }
    }
    return result;
}

// Constructor and destructor
Client::Client() {
    //std::cout << "Construction client par defaut" << std::endl;
}

Client::~Client() {}

// Class function
void Client::setData(char *buffer) {
    std::string test = buffer;
    // std::vector<std::string> res;
    // std::istringstream f(test);
    // std::string s;
    // while (getline(f, s, "\r\n")) {
    //     if (s.rfind("NICK", 0) == 0) {
    //         std::cout << "'"<< buffer << "'" << std::endl;
    //         _nickname = s.substr(5, 6);
    //     }
    // }
    std::cout << "va chier '" << t(test) << std::endl;
    if (test.find("NICK", 0) == 0) {
        std::cout << "'" << t(test) << "'" << std::endl;
        std::cout << "conard " << test.substr(5) << std::endl;
    }
    //std::cout << "debug client nickname : " << _nickname << ", fd : " << _fd << " ." << std::endl;
}

// Getters and setters
int Client::getFd() {
    return _fd;
}

std::string Client::getNickname() {
    return _nickname;
}

void Client::setFd(int value) {
    _fd = value;
}

void Client::setNickname(std::string value) {
    _nickname = value;
}