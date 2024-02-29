#include "Client.hpp"

// Constructor and destructor
Client::Client(int fd, std::string name) {
    _fd = fd;
    _name = name;
}

Client::~Client() {
    std::cout << "Destruction client nb : " << _fd << " : " << _name << "." << std::endl;
}

// Class function
void Client::readClient() {

}

// Getters and setters
int Client::getFd() {
    return _fd;
}

std::string Client::getName() {
    return _name;
}

void Client::setName(std::string value) {
    _name = value;
}