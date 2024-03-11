/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugu <aaugu@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:39:02 by aaugu             #+#    #+#             */
/*   Updated: 2024/03/11 20:49:43 by aaugu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

# include "../includes/Server.hpp"
# include "../includes/errorHandling.hpp"
# include "../includes/signal.hpp"
# include "../includes/Client.hpp"

/* ************************************************************************** */
/*                          CONSTRUCTORS & DESTRUCTOR                         */
/* ************************************************************************** */

Server::Server(int port) : _nbConnections(0)
{
	std::cout << "Initializing server..." << std::endl;

	// Create an AF_INET6 stream socket to receive incoming connections on
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
		throw std::runtime_error(errMessage("Server1", -1, strerror(errno)));

	// Allow socket descriptor to be reuseable
	int optionValue = 1;
	if ( setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEADDR, &optionValue, sizeof(optionValue)) == -1) {
		close(_sockfd);
		throw std::runtime_error(errMessage("Server2", -1, strerror(errno)));
	}

	// Set socket to be nonblocking. All of the sockets for the incoming connections
	// will also be nonblocking since they will inherit that state from the listening socket
	// if ( fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1 ) {
	// 	close(_sockfd);
	// 	throw std::runtime_error(errMessage("Server3", -1, strerror(errno)));
	// }

	// Bind the socket
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ( bind(_sockfd, (const struct sockaddr *)&_addr, sizeof(_addr)) == -1) {
		close(_sockfd);
		throw std::runtime_error(errMessage("Server4", -1, strerror(errno)));
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
		for ( it = _pollFds.begin() + 1; it < _pollFds.end(); it++ )
		{
			std::cout << it->fd << " bouh\n";
			if ( it->revents & POLLIN )
			{
			// 	if ( it->fd == _sockfd ) // server side
			// 		createClientConnection();
			// 	else  // client side
			// 	{
					std::string clientInput = "";
					getClientInput(it, clientInput);
					if (!clientInput.empty())
						//continue;
					// whatever whatever = parseClientInput(clientInput);
						executeClientInput(clientInput, it->fd); // whatever à ajouter dans les paramètres
				// }
			}
		}
	}
}

void Server::stop(void)
{
	// this->run = 0;
	closePollFds();
	std::cerr << "!!!! Serveur STOP !!!!" << std::endl;
}


/* ************************************************************************** */
/*                              PRIVATE FUNCTIONS                             */
/* ************************************************************************** */

// ---------------------- Main function sub functions ----------------------- //

void	Server::setListenBackLog(void)
{
	if ( listen(_sockfd, MAXCLIENT + 1) == -1 )
		throw std::runtime_error(errMessage("Server5", -1, strerror(errno)));

	pollfd	listenFd;
	listenFd.fd = _sockfd;
	listenFd.events = POLLIN;

	_pollFds.push_back(listenFd);
}

void	Server::waitForEvent(void)
{
	int	timeout = -1;

	if ( poll(&_pollFds[0], (nfds_t) _nbConnections + 1, timeout) == -1 && sig::stopServer == false)
		throw std::runtime_error(errMessage("Server6", -1, strerror(errno)));
}

void	Server::createClientConnection(void)
{
	int	sockfdClient;
	char	hostname_c[1024];
	int		return_number = gethostname(hostname_c, 1024);

	std::cout << "hostname_c = " << hostname_c << " return_number = " << return_number << std::endl;

	sockfdClient = acceptNewClient();

	if (_nbConnections + 1 > MAXCLIENT)
		return (refuseClient(sockfdClient));

	addClientToListenPoll(sockfdClient);
	_clients.push_back(Client(sockfdClient));
	std::cout << "nb client :" << _clients.size() << std::endl;

	std::cout	<< "New connection : "
				<< "[SOCKET_FD : "	<< sockfdClient
				<< " , IP : "		<< inet_ntoa(_addr.sin_addr)
				<< " , PORT : "		<< ntohs(_addr.sin_port) << "]"
				<< std::endl;

	send(sockfdClient, "Welcome\n", 8, 0);
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

static int get_line(int fd, std::string &line){
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
		return (readed);
	return total_read;
}

void	Server::getClientInput(std::vector<pollfd>::iterator clientPollFd, std::string& clientInput)
{
	// char	buffer[4096] = {0};
	// size_t	readBytes = recv(clientPollFd->fd, buffer, 4096, 0);
	// infos irssi à récup ?

	std::string	line;
	size_t readBytes = get_line(clientPollFd->fd, line);
	clientInput = line;

	if ( (int)readBytes == -1 )
		throw std::runtime_error(errMessage("Server7", clientPollFd->fd, strerror(errno)));
	else if (readBytes == 0)
		return (disconnectClient(clientPollFd));
	else
	{
		// if (clientInput == "JOIN :\r\n")
		// {
		// 	const char* response = ":c2r9s3.42lausanne.ch 451 test\n";
		// 	send(clientPollFd->fd, response, strlen(response), 0);
		// 	std::cerr << "SEND: " << t(response) << std::endl;
		// }
		// if (clientInput == "NICK lvogt\r\n")
		// {
		// 	const char* response = ":c2r9s3.42lausanne.ch 001 lvogt :Welcome to the Internet Relay Network lvogt!lvogt@127.0.0.1\n";
		// 	send(clientPollFd->fd, response, strlen(response), 0);
		// 	std::cerr << "SEND: " << t(response) << std::endl;
		// }
		// buffer[readBytes] = '\0';
		// clientInput = static_cast<std::string>(buffer);
		return ;
	}
}

void	Server::executeClientInput(std::string clientInput, int sockfdClient)
{
	std::vector<Client>::iterator itC = getClientByFd(sockfdClient);
	if ( itC == _clients.end() )
		return (printErrMessage(errMessage("Client", sockfdClient, "could not find client with this fd")));

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

	if (sockfdClient == -1)
		throw std::runtime_error(errMessage("Client : ", sockfdClient, strerror(errno)));

	return (sockfdClient);
}

void	Server::refuseClient(int sockfdClient)
{
	send(sockfdClient, "ERROR : le nombre maximum de client a ete atteint sur ce serveur\n", 65, 0);

	if (close(sockfdClient) == -1)
		throw std::runtime_error(errMessage("Client", sockfdClient, strerror(errno)));

	return (printErrMessage(errMessage("Server", -1, "cannot accept more client")));
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
	std::cout << "clients size : "  <<  _clients.size() << std::endl << "pollfds size : " << _pollFds.size() << std::endl;

	std::cout << "Client " << pollfd->fd << ": disconnected" << std::endl;
	if (close(pollfd->fd) == -1)
		throw std::runtime_error(errMessage("Client", pollfd->fd, strerror(errno)));

	_nbConnections--;

	std::vector<Client>::iterator itC = getClientByFd(pollfd->fd);
	if ( itC == _clients.end() )
		return (printErrMessage(errMessage("Client", pollfd->fd, "could not find client with this fd")));

	_pollFds.erase(pollfd);
	_clients.erase(itC);

	std::cout << "clients size : " <<  _clients.size() << std::endl << "pollfds size : " << _pollFds.size() << std::endl;
}

// ---------------------------- Stop signal utils --------------------------- //

void    Server::closePollFds(void)
{
	std::vector<pollfd>::iterator	it;
	int	sockfd;

	for (it = _pollFds.begin(); it != _pollFds.end(); it++)
	{
		sockfd = (*it).fd;
		if (sockfd > 0)
		{
			if (close(sockfd) == -1)
				throw std::runtime_error(errMessage("Client", sockfd, strerror(errno)));
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
/*                                     DEBUG                                  */
/* ************************************************************************** */

void Server::printNickname() {
	std::vector<Client>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		std::cout << "--->" << it->getNickname() << "<---" << it->getFd() << " <---- "<< std::endl;
	}
}
