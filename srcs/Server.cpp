/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/12 12:15:06 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

# include "../includes/Server.hpp"
# include "../includes/messages.hpp"
# include "../includes/signal.hpp"
# include "../includes/Client.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTOR & DESTRUCTOR                          */
/* ************************************************************************** */

Server::Server(int port) : _nbConnections(0)
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
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) < 0) {
		close(_sockfd);
		throw std::runtime_error(errMessage(ERR_SOCK_BIND, -1, strerror(errno)));
	}

	// Signals
	signal( SIGINT, sig::signalHandler );

	std::cout << "Server successfully initialized! Listening for connections on port " << port << std::endl;
}

Server::~Server(void) {
	std::cout << "au revoir" << std::endl;
}

/* ************************************************************************** */
/*                              PUBLIC FUNCTIONS                              */
/* ************************************************************************** */

void Server::run(void)
{
	setListenBackLog();

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
					// whatever whatever = parseClientInput(clientInput);
					executeClientInput(clientInput, it->fd); // whatever à ajouter dans les paramètres
				}
			}
		}
	}
}

void Server::stop(void)
{
	closePollFds();
	std::cerr << "!!!! Serveur STOP !!!!" << std::endl;
}


/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

// ---------------------- Main function sub functions ----------------------- //

void	Server::setListenBackLog(void)
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

	if ( poll(&_pollFds[0], (nfds_t) _nbConnections + 1, timeout) < 0 && sig::stopServer == false)
		throw std::runtime_error(errMessage(ERR_POLL, -1, strerror(errno)));
}

void	Server::createClientConnection(void)
{
	int		sockfdClient;
	/* TEST / DEBUG
	char	hostname_c[1024];
	int		return_number = gethostname(hostname_c, 1024);

	std::cout << "hostname_c = " << hostname_c << " return_number = " << return_number << std::endl;
	*/

	sockfdClient = acceptNewClient();

	if (_nbConnections + 1 > MAXCLIENT)
		return (refuseClient(sockfdClient));

	addClientToListenPoll(sockfdClient);
	_clients.push_back(Client(sockfdClient));

	std::cout	<< "New connection : "
				<< "[SOCKET_FD : "	<< sockfdClient
				<< " , IP : "		<< inet_ntoa(_addr.sin_addr)
				<< " , PORT : "		<< ntohs(_addr.sin_port) << "]"
				<< std::endl;

	sendMessage("Welcome", sockfdClient);
}

void	Server::getClientInput(std::vector<pollfd>::iterator clientPollFd, std::string& clientInput)
{
	std::string	line;
	size_t readBytes = getLine(clientPollFd->fd, clientInput);

	if ( (int)readBytes < 0 )
		printErrMessage(errMessage("Server7", clientPollFd->fd, strerror(errno))); // à changer pour gérer CTRL + D
	else if (readBytes == 0)
		disconnectClient(clientPollFd);
}

void	Server::executeClientInput(std::string clientInput, int sockfdClient)
{
	std::vector<Client>::iterator itC = getClientByFd(sockfdClient);
	if ( itC == _clients.end() )
		return (printErrMessage(errMessage("Client", sockfdClient, ERR_CLIENT_NONEX)));

	itC->setData(clientInput);

	std::cout << "Client " << sockfdClient << ": " << clientInput;
}

// ------------------------------ Client Utils ------------------------------ //

int		Server::acceptNewClient(void)
{
	int					sockfdClient;
	struct sockaddr_in	addrClient;
	socklen_t			addrLenClient = sizeof(addrClient);

	sockfdClient = accept(_sockfd, (struct sockaddr *)&addrClient, &addrLenClient);

	if (sockfdClient < 0)
		throw std::runtime_error(errMessage(ERR_CLIENT_ACCEPT, sockfdClient, strerror(errno)));

	return (sockfdClient);
}

void	Server::refuseClient(int sockfdClient)
{
	sendMessage(MAX_CONNECTIONS, sockfdClient);

	if (close(sockfdClient) < 0)
		throw std::runtime_error(errMessage(ERR_CLOSE, sockfdClient, strerror(errno)));

	printErrMessage(errMessage("Client", -1, SERVER_FULL));
}

void	Server::addClientToListenPoll(int sockfdClient)
{
	pollfd	client;

	client.fd = sockfdClient;
	client.events = POLLIN | POLLOUT;

	_pollFds.push_back(client);
	_nbConnections++;
}

void	Server::disconnectClient(std::vector<pollfd>::iterator pollfd)
{
	std::cout << "Client " << pollfd->fd << ": disconnected" << std::endl;
	if (close(pollfd->fd) < 0)
		throw std::runtime_error(errMessage(ERR_CLOSE, pollfd->fd, strerror(errno)));

	_nbConnections--;

	std::vector<Client>::iterator itC = getClientByFd(pollfd->fd);
	if ( itC == _clients.end() )
		return (printErrMessage(errMessage("Client", pollfd->fd, ERR_CLIENT_NONEX)));

	_pollFds.erase(pollfd);
	_clients.erase(itC);
}

// ---------------------------- Stop signal utils --------------------------- //

void    Server::closePollFds(void)
{
	std::vector<pollfd>::iterator	it;
	int	sockfd;

	for (it = _pollFds.begin(); it != _pollFds.end(); it++)
	{
		sockfd = it->fd;
		if (sockfd > 0)
		{
			if (close(sockfd) < 0)
				throw std::runtime_error(errMessage(ERR_CLOSE, sockfd, strerror(errno)));
		}
	}
}

/* ************************************************************************** */
/*                                  ACCESSORS                                 */
/* ************************************************************************** */

std::vector<Client>::iterator	Server::getClientByFd(int sockfdClient)
{
	std::vector<Client>::iterator it;
	for ( it = _clients.begin(); it != _clients.end(); it++ )
	{
		if ( it->getFd() == sockfdClient)
			return (it);
	}
	return (it);
}

/* ************************************************************************** */
/*                                     UTILS                                  */
/* ************************************************************************** */

int Server::getLine(int fd, std::string &line)
{
	char chr[2] = {0};
	int readed = 0;
	int total_read = 0;;
	while ((readed = recv(fd,chr, 1, 0)) > 0)
	{
		total_read += readed;
		std::string	append(chr);
		line += append;
		if (chr[0] == '\n')
			break;
		memset(chr, 0, 2);
	}
	if (readed < 0)
		total_read = -1;
	return total_read;
}

/* ************************************************************************** */
/*                                     DEBUG                                  */
/* ************************************************************************** */

void Server::printNickname() {
	std::vector<Client>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		std::cout << "--->" << it->getNickname() << "<---" << it->getFd() << " <---- "<< std::endl;
	}
}

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