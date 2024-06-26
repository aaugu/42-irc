/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/04/08 09:45:18 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "../includes/Server.hpp"
#include "../includes/messages.hpp"
#include "../includes/signal.hpp"
#include "../includes/Client.hpp"

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

#define MAXCLIENT 			5
#define OPPASS				"pass"
#define ERR_SOCK_CREATE		"Could not create socket"
#define ERR_SOCK_OPT		"Could not set socket option"
#define ERR_SOCK_NON_BLOCK	"Could not set sockets to be non blocking"
#define ERR_SOCK_BIND		"Could not bind socket"
#define ERR_SOCK_LISTEN		"Could not listen to the socket"
#define ERR_POLL			"Problem while waiting for fd to perform"
#define ERR_CLIENT_ACCEPT	"Could not create connection with client"

#define IP_ADDRESS          "127.0.0.1"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Server::Server(int port, std::string password) : _nbConnections(0), _opPass(OPPASS)
{
	std::cout << "Initializing server..." << std::endl;

	// Create an AF_INET6 stream socket to receive incoming connections on
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
		throw std::runtime_error(errMessage(ERR_SOCK_CREATE, -1, strerror(errno)));

	// Allow socket descriptor to be reuseable
	int optionValue = 1;
	if ( setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEADDR, &optionValue, sizeof(optionValue)) < 0) {
		close(_sockfd);
		throw std::runtime_error(errMessage(ERR_SOCK_OPT, -1, strerror(errno)));
	}

	// Set socket to be nonblocking. All of the sockets for the incoming connections
	// will also be nonblocking since they will inherit that state from the listening socket
	if ( fcntl(_sockfd, F_SETFL, O_NONBLOCK) < 0 ) {
		close(_sockfd);
		throw std::runtime_error(errMessage(ERR_SOCK_NON_BLOCK, -1, strerror(errno)));
	}

	// Bind the socket
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	if ( bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) < 0) {
		close(_sockfd);
		throw std::runtime_error(errMessage(ERR_SOCK_BIND, -1, strerror(errno)));
	}
	// Signals
	signal( SIGINT, sig::signalHandler );
	_password = password;
	std::cout << "Server successfully initialized! Listening for connections on port " << port << std::endl;
}

Server::~Server(void) {
	disconnectAllClients();
	std::cerr << "!!!! Serveur STOP !!!!" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

// ------------------------- Server Main function  -------------------------- //
void Server::run(void)
{
	startServer();

	while (sig::stopServer == false)
	{
		waitForEvent();

		if ( _pollFds.front().revents & POLLIN ) // server side
			createClientConnection();

		std::vector<pollfd>::iterator it;
		for ( it = _pollFds.begin() + 1; it < _pollFds.end(); it++ ) // client side
		{
			if ( it->revents & POLLIN && it->revents > 0)
			{
				std::string clientInput = "";
				getClientInput(it, clientInput);
				if (clientInput.empty() == false)
				{
					parseClientInput(clientInput, it->fd);
					executeClientInput(*this, it);
				}
			}
		}
	}
}

/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

// ----------------------- Server Main function Utils ----------------------- //
void	Server::startServer(void)
{
	if ( listen(_sockfd, MAXCLIENT + 1) < 0 )
		throw std::runtime_error(errMessage(ERR_SOCK_LISTEN, -1, strerror(errno)));

	pollfd	listenFd;
	listenFd.fd = _sockfd;
	listenFd.events = POLLIN;

	_pollFds.push_back(listenFd);
}

void	Server::waitForEvent(void)
{
	int	timeout = 0;

	if ( poll(&_pollFds[0], (nfds_t) _nbConnections + 1, timeout) == -1 && sig::stopServer == false)
		throw std::runtime_error(errMessage("Server : ", -1, strerror(errno)));
}

void	Server::createClientConnection(void)
{
	int		sockfdClient;

	sockfdClient = acceptNewClient();

	if (_nbConnections + 1 > MAXCLIENT)
		return (refuseClient(sockfdClient));

	createClient(sockfdClient);
	addClientToListenPoll(sockfdClient);
	_nbConnections++;

	std::cout	<< "New connection : "
				<< "[SOCKET_FD : "	<< sockfdClient
				<< " , IP : "		<< inet_ntoa(_addr.sin_addr)
				<< " , PORT : "		<< ntohs(_addr.sin_port) << "]"
				<< std::endl;
}

void	Server::getClientInput(std::vector<pollfd>::iterator clientPollFd, std::string& clientInput)
{
	std::vector<Client*>::iterator itC = getClientByFd(clientPollFd->fd);
	std::string	line;
	size_t readBytes = getLine(clientPollFd->fd, line);
	std::cout << "\nclient " << (*itC)->getFd() << " " << t(line) << std::endl;

	if ( (int)readBytes < 0 && line.empty() == false){
		std::cerr << "WAIT finish command" << std::endl; // debug
		(*itC)->saveMessage(line);
		sendMessageTo("^D", (*itC)->getFd()); // pour le visuel client
	}
	else if ( (int)readBytes < 0 )
		printErrMessage("error when read client buffer.");
	else if (readBytes == 0)
		clientInput = "QUIT :lost connexion\r\n";
	else
		clientInput = line;
}

void	Server::parseClientInput(std::string clientInput, int sockfdClient)
{
	std::vector<Client*>::iterator itC = getClientByFd(sockfdClient);
	if ( itC == _clients.end() )
		return (printErrMessage(errMessage("Client", sockfdClient, ERR_CLIENT_NONEX)));

	(*itC)->parseMessage(clientInput);
}

void	Server::executeClientInput(Server &server, std::vector<pollfd>::iterator it)
{
	std::vector<Client*>::iterator itC = getClientByFd(it->fd);
	if ( itC == _clients.end() )
		return (printErrMessage(errMessage("Client", it->fd, ERR_CLIENT_NONEX)));

	(*itC)->exeCommand(&server);
}


// ---------------------------- Stop signal utils --------------------------- //
void	Server::disconnectAllClients(void)
{
	std::vector<Client*>::iterator it;
	for ( it = _clients.begin(); it < _clients.end(); it++ )
		disconnectClient(*it);
}

/* ************************************************************************** */
/*                                  ACCESSORS                                 */
/* ************************************************************************** */

std::vector<pollfd>::iterator Server::getPollFdByFd(int sockfd) {
	std::vector<pollfd>::iterator it;
	for ( it = _pollFds.begin(); it < _pollFds.end(); it++ )
	{
		if ( it->fd == sockfd)
			return (it);
	}
	return (it);
}

std::vector<std::string> Server::getNicknameList() {
    std::vector<Client*>::iterator it;
    std::vector<std::string> nickname;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        nickname.push_back((*it)->getNickname());
    }
    return nickname;
}

std::string Server::getOpPass() {
    return _opPass;
}

std::string Server::get_password() const {
    return _password;
}

std::vector<Client*> Server::getClients(void) {
	return _clients;
}

std::vector<Channel*>	Server::getChannels(void) {
	return ( _channels );
}

/* ************************************************************************** */
/*                                     UTILS                                  */
/* ************************************************************************** */


int Server::getLine(int fd, std::string &line)
{
	char chr[2] = {0};
	int readed = 0;
	int total_read = 0;;
	while ((readed = recv(fd,chr, 1, 0)) > 0){
		total_read += readed;
		std::string append(chr);
		line += append;
		if (chr[0] == '\n')
			break;
		memset(chr, 0, 2);
	}
	if(readed < 0){
		total_read = -1;
		std::string append(chr);
		line += append;
		memset(chr, 0, 2);
	}
	return total_read;
}

/* ************************************************************************** */
/*                                     DEBUG                                  */
/* ************************************************************************** */

void Server::printNickname() {
	std::vector<Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		std::cout << "--->" << (*it)->getNickname() << "<---" << (*it)->getFd() << " <---- "<< std::endl;
	}
}

std::string Server::t(const std::string& input) {
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
