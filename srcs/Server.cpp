#include <list>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <err.h>
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include <poll.h> // Ajout de cette inclusion
#include "../includes/Server.hpp"
#include "../includes/error_handling.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Server::Server(void) {}

Server::Server(int port) : nbConnections(0)
{
    std::cout << "Initializing server..." << std::endl;

    // Create an AF_INET6 stream socket to receive incoming connections on
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd == -1)
        throw std::runtime_error(errMessage("Server : ", strerror(errno)));

    // Allow socket descriptor to be reuseable
    int on = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
        throw std::runtime_error(errMessage("Server : ", strerror(errno)));

    // Set socket to be nonblocking. All of the sockets for the incoming connections
    // will also be nonblocking since they will inherit that state from the listening socket
    int flags = fcntl(_sockfd, F_GETFL, 0);
    if (fcntl(_sockfd, F_SETFL, flags | O_NONBLOCK) == -1) // Modification ici
        throw std::runtime_error(errMessage("Server : ", strerror(errno)));

    // Bind the socket
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) == -1)
        throw std::runtime_error(errMessage("Server : ", strerror(errno)));

    std::cout << "Server successfully initialized!" << std::endl;
}

Server::~Server(void)
{
    // closePollFds();
    // delete [] _pollFds;
    // std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::start(void)
{
    // Set the listen back log
    int ls = listen(_sockfd, SOMAXCONN);
    if (ls == -1)
        throw std::runtime_error(errMessage("Server : ", strerror(errno)));

    std::cout << _sockfd << std::endl;

    _pollFds[0].fd = _sockfd;
    _pollFds[0].events = POLLIN;

    int sockfdClient;
    struct sockaddr_in addrClient;
    socklen_t addrLenClient;

    char buffer[1024] = {0};

    while (true)
    {
        waitForEvent();

        if (_pollFds[0].revents & POLLIN)
        {
            addrLenClient = sizeof(addrClient);
            sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, &addrLenClient);

            if (sockfdClient == -1)
                throw std::runtime_error(errMessage("Client : ", strerror(errno)));
            else
            {
                _pollFds[nbConnections + 1].fd = sockfdClient;
                _pollFds[nbConnections + 1].events = POLLIN;
                nbConnections++;

                std::cout << "Client " << sockfdClient << " connected." << std::endl;
                send(sockfdClient, "Welcome\n", 8, 0);
            }
        }

        for (int i = 1; i < nbConnections + 1; i++)
        {
            if (_pollFds[i].revents & POLLIN)
            {
                ssize_t bytesRead = recv(_pollFds[i].fd, buffer, sizeof(buffer), 0);
                if (bytesRead > 0)
                {
                    buffer[bytesRead] = '\0'; 
                    std::cout << "Received from client " << _pollFds[i].fd << ": " << buffer << std::endl;
                }
                else if (bytesRead == 0)
                {
                    std::cout << "Client " << _pollFds[i].fd << " disconnected." << std::endl;
                    close(_pollFds[i].fd);
                    for (int j = i; j < nbConnections; ++j)
                        _pollFds[j] = _pollFds[j + 1];
                    --nbConnections;
					std::cout << "debug nb client : " << nbConnections << std::endl;
                }
                else
                {
                    std::cerr << "Error receiving data from client " << _pollFds[i].fd << std::endl;
                }
            }
        }
    }
}

void Server::stop(void)
{
    this->run = 0;
    // std::for_each (_clients.begin(), _clients.end(), closeClient);
}

/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

void Server::waitForEvent(void)
{
    int timeout = 0;
    if (poll(_pollFds, nbConnections + 1, timeout) == -1)
        throw std::runtime_error(errMessage("Server : ", strerror(errno)));
}

/* ************************************************************************** */
/*                            NON MEMBER FUNCTIONS                            */
/* ************************************************************************** */

// ---------------------------- Destructor utils ---------------------------- //
void Server::closePollFds(void)
{
    // std::vector<pollfd>                fds = *_pollFds;
    // std::vector<pollfd>::iterator    it;
    // int    sockfd;
    // int i = 0;

    // for (it = fds.begin(); i < nbConnections + 1; it++, i++)
    // {
    //    sockfd = (*it).fd;
    //    if (sockfd > 0)
    //        close(sockfd);
    // }
}
