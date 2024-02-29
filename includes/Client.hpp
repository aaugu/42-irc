#pragma once

#include "Server.hpp"

class Client {
    public:
    // Constructor and destructor
    Client(int fd, std::string name);
    ~Client();

    // Class function
    void readClient();

    // Getters and setters
    int getFd();
    std::string getName();
    void setName(std::string value);

    private:
    int _fd;
    std::string _name;
};
