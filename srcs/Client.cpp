#include "Client.hpp"

// Constructor and destructor
Client::Client(int fd, std::string name): _fd(fd), _name(name) {
}

Client::~Client() {
    std::cout << "Destruction client nb : " << _fd << " : " << _name << "." << std::endl;
}

// Class function
void Client::setData(int fd, char *buffer) {
    (void)fd;
    (void)buffer;
    std::string test = buffer;
    std::vector<std::string> res;
    std::istringstream f(test);
    std::string s;
    while (getline(f, s, '\n')) {
        if (s.rfind("NICK", 0) == 0)
            std::cout << "nickname " << s.substr(5) << std::endl;
        res.push_back(s);
    }
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