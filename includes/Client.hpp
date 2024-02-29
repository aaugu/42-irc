#pragma once

#include "Server.hpp"

class Client {
    public:
    // Constructor and destructor
    Client();
    ~Client();

    // Class function
    void setData(char *data);

    // Getters and setters
    int getFd();
    std::string getNickname();

    void setFd(int value);
    void setNickname(std::string value);

    private:
    int _fd;
    std::string _nickname;
};
