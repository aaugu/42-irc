#include "Client.hpp"

std::vector<std::string> split(std::string value) {
    std::istringstream iss(value);
    std::vector<std::string> mots;
    std::string mot;

    while (iss >> mot) {
        mots.push_back(mot);
    }
    return mots;
}

// Constructor and destructor
Client::Client() {
    _nickname = "G'raha Tia";
    std::cout << "Construction client par defaut" << std::endl;
}

Client::~Client() {}

// Class function
void Client::setData(char *buffer) {
    std::string value = buffer;
    std::vector<std::string> info = split(value);
    std::cout << _nickname << std::endl;
    if (_nickname == "G'raha Tia")
        _nickname = info[1];
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