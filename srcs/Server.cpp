/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:20:36 by lchan             #+#    #+#             */
/*   Updated: 2022/12/09 15:56:22 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*********************************************
 * 				Coplien Form
 *********************************************/

Server::Server()
	: _port(SERVER_DEFAULT_PORT), _password(), _name(SERVER_NAME), _creationTime(getTimeStr()),
	  _addrlen(sizeof(_sockAddr)), _listenSd(-1), _status(OFF_STATUS),
	  _condenceArrayFlag(ON_STATUS), _opt(1), _nfds(0), _newSd(0)
{
	ircMemset((void *)_buffer, 0, sizeof(_buffer));
	ircMemset((void *)_fds, 0, sizeof(_fds));
	std::cout << _creationTime << "Server constructor called" << std::endl;
	std::cout << "password = " << _password << std::endl;
	std::cout << "port = " << _port << std::endl;
}

Server::Server(int port, std::string pwd)
	: _port(port), _password(pwd), _name(SERVER_NAME), _creationTime(getTimeStr()),
	  _addrlen(sizeof(_sockAddr)), _listenSd(-1), _status(OFF_STATUS),
	  _condenceArrayFlag(ON_STATUS), _opt(1), _nfds(0), _newSd(0)
{
	ircMemset((void *)_buffer, 0, sizeof(_buffer));
	ircMemset((void *)_fds, 0, sizeof(_fds));
	std::cout << _creationTime << " Server constructor called" << std::endl;
	std::cout << "password = " << _password << std::endl;
	std::cout << "port = " << _port << std::endl;
}

Server::~Server()
{
	closeAllConn();
	std::cout << "Server destructor called" << std::endl;
}

/*********************************************
 * 				initServer
 *********************************************/

/*************************************************************
 * Create stream socket to receive incoming connections
 *************************************************************/
int Server::setSocket()
{
	_listenSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_listenSd == ERROR)
		return (E_SOCKET_ERR);
	else
		std::cout << "1. socket success" << std::endl;
	return (E_SOCK_SUCCESS);
}

/*************************************************************
 * Allow socket descriptor to be reuseable
 *************************************************************/
int Server::setSocketopt()
{
	socklen_t	val = 1;

	if (setsockopt(_listenSd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &val, sizeof(socklen_t)) == ERROR)
		return (E_SETSOCKOPT_ERR);
	std::cout << "2. socketopt success" << std::endl;
	return (E_SOCK_SUCCESS);
}

/*************************************************************
 * Set socket to be nonblocking. (FIONDBIO )
 * 	--> (similar to O_NONBLOCK flag with the fcntl subroutine)
 * All incoming connections will inherit that state from listening socket
 *************************************************************/
int Server::setNonBlocking()
{
	int on = 1;
	int ret = ioctl(_listenSd, FIONBIO, (char *)&on);
	if (ret < 0)
		return (E_SETNONBLOKING_ERR);
	std::cout << "3. setNonBlocking success" << std::endl;
	return (E_SOCK_SUCCESS);
}

/*************************************************************
 * Bind the socket
 *************************************************************/
int Server::bindSocket()
{
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr.s_addr = INADDR_ANY;//inet_addr(LOCAL_HOST);
	_sockAddr.sin_port = htons(_port);
	if ((bind(_listenSd, (sockaddr *)&_sockAddr, _addrlen)) == ERROR)
		return (E_BIND_ERR);
	std::cout << "4. bind success" << std::endl;
	return (E_SOCK_SUCCESS);
}

/*************************************************************
 * Set the listen back log and initial listening socket
 *************************************************************/
int Server::setListenSocket()
{
	if (listen(_listenSd, MAX_CLIENT) == ERROR)
		return (E_LISTEN_ERR);
	_fds[0].fd = _listenSd;
	_fds[0].events = POLLIN;
	std::cout << "5. listening" << std::endl;
	return (E_SOCK_SUCCESS);
}

/*************************************************************
 * Server initialisation function
 *************************************************************/
void Server::initServer()
{
	std::string names[5] = {
		"socket",
		"socketopt",
		"setNonBlocking"
		"bind",
		"listen",
	};
	int (Server::*funkTab[5])() = {
		&Server::setSocket,
		&Server::setSocketopt,
		&Server::setNonBlocking,
		&Server::bindSocket,
		&Server::setListenSocket};

	for (int i = 0; i < 5; i++)
		if ((this->*funkTab[i])() == ERROR)
			throw std::invalid_argument(names[i] + " error");
	_status = ON_STATUS;
	_nfds++;
	serverPrint(SERVER_START_MESS);
}

/*************************************************************
 * Poll / wait for newconn
 *************************************************************/
int Server::checkPollRet(int ret)
{
	switch (ret)
	{
		case -1:
			serverPrint(POLL_ERR_MESS);
			return (POLL_FAILURE);
		case 0:
			serverPrint(TIMEOUT_MESS);
			return (POLL_FAILURE);
		default:
			_status = ON_STATUS;
			return (POLL_OK);
	}
}

int Server::findReadableFd()
{
	for (int index = 0; index < _nfds; index++)
	{
		if (_fds[index].revents == 0)
			continue;					   // do next loop,
		if (_fds[index].revents != POLLIN)
		{
			//_status = OFF_STATUS;
			return (POLL_FAILURE);
		}
		// else if (_fds[index].revents == POLLHUP)
		// 	printf("deconnection spoted");
		else
		{
			printf("[DEBUG_MESS] findReadableFd : returning : = %d \n", index);
			return (index);
		}
	}
	return (POLL_FAILURE);
}

/****************************************************************************
Accept all incoming connections that are queued up on the listening socket
before we loop back and call poll again.
*****************************************************************************/
int Server::acceptNewSd()
{
	_newSd = accept(_listenSd, NULL, NULL); // ckeck on man to see if other argument are necessary or not

	if (_newSd < 0)
	{
		if (errno != EWOULDBLOCK)
			return (turnOffServer("accept() failed"));
	}
	else
	{
		_fds[_nfds].fd = _newSd; // we can directly use _nfds as an index coz we compressArray at each single connexion
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
	return (POLL_OK);
}

/****************************************************************************
	receive all incoming data that send by _fds[index]
	before we loop back and call poll again.
	recv returns the number of bytes received, -1 if an error occured and 0 for EOF
	*****************************************************************************/
void Server::readExistingFds(int index)
{
	// printf("readExistingFds : fd is readable, a new message has been received: index = %d , fd = %d \n", index, _fds[index].fd);

	int recvRet;

	recvRet = recv(_fds[index].fd, _buffer, sizeof(_buffer), 0);
	if (recvRet < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			serverPrint("recv() failed");
			closeConn(index);
		}
	}
	else if (recvRet == 0) // not sure about this one.
		closeConn(index);
	else
		handleCmd(_fds[index].fd);
	ircMemset((void *)_buffer, 0, sizeof(_buffer));
}

void Server::sendMsg(int fd, std::string &msg)
{
	int sendRet;

	msg = ":" + _name + " " + msg + _CRLF;
//	std::cout << "\033[31m]==================== here 1: \033[0m" << msg << std::endl;

	sendRet = send(fd, msg.c_str(), msg.length(), 0);
	if (sendRet < 0)
	{
		serverPrint("send() failed");
		for (int i = 0; i < _nfds; i++)
			if (_fds[i].fd == fd)
				closeConn(i);
	}
}

void Server::sendMessage(int fd, std::string id, std::string &msg)
{
	int sendRet;

	msg = ":" + id + " " + msg + _CRLF;
	sendRet = send(fd, msg.c_str(), msg.length(), 0);
	if (sendRet < 0)
	{
		serverPrint("send() failed");
		for (int i = 0; i < _nfds; i++)
			if (_fds[i].fd == fd)
				closeConn(i);
	}
}

void Server::sendToAllUser(std::string &msg)
{
	userMap::iterator it = _userMap.begin();

	for (; it != _userMap.end(); ++it)
		sendMsg((*it).first, msg);
}

/****************************************************************************
 * if poll does not fail, it's either :
	-> _listenSd (listen socket descriptor) is readable (a new connection is incomming)
	-> _fds[i] (an existing connection) is readable
 ****************************************************************************/
void Server::reactToEvent(int index)
{

	if (index == POLL_FAILURE)
		return;
	else if (_fds[index].fd == _listenSd)
		acceptNewSd();
	else
		readExistingFds(index);
}

/*******************************************
* poll is blocking until
	-> it catchs an event(new fds or listening socket is readable)
	-> it fails
	-> it times out
* poll has to be in the loop (recalled each time). it actes like a reset
********************************************/
void Server::waitForConn()
{

	do
	{
		if (checkPollRet(poll(_fds, _nfds, TIMEOUT)) == POLL_FAILURE)
			break;
		else
			reactToEvent(findReadableFd());
	} while (_status == ON_STATUS);
	closeAllConn();
}

void Server::startServer()
{
	try
	{
		initServer();
		waitForConn();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

/******************************************
	Server management Utils
*******************************************/
void Server::closeConn(int index)
{
	int fd = _fds[index].fd;

	if (fd > -1)
	{
		deleteUser(fd);
		close(fd);
		_fds[index].fd = -1;
	}
	if (_condenceArrayFlag && index < _nfds)
		NarrowArray();
}

void Server::closeFd(int targetFd)
{
    for (int i = 0; i < _nfds; i++)
        if (_fds[i].fd == targetFd)
            closeConn(i);
}

void Server::NarrowArray(void)
{
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].fd == -1)
		{
			for (int j = i; j < _nfds; j++)
				_fds[j].fd = _fds[j + 1].fd;
			i--;
			_nfds--;
		}
	}
}



void Server::deleteUser(int fd)
{
	userMap::iterator					userIt;
	cmdMap::iterator					cmdIt;
	userDirectory::iterator				usDirIt;
	std::vector<Channel *>::iterator	chanIt;

	userIt = _userMap.find(fd);
	if (userIt != _userMap.end())
	{
		//remove all from chan
		for (size_t i(0); i < _channels.size(); ++i)
			_channels[i]->part(_userMap[fd]);
		//delete from _userMap
		delete (_userMap[fd]);
		_userMap.erase(fd);
	}
	// delete from _cmdMap
	cmdIt = _cmdMap.find(fd);
	if (cmdIt != _cmdMap.end())
		_cmdMap.erase(fd);
}

int Server::turnOffServer(std::string str)
{
	serverPrint(str);
	_status = OFF_STATUS;
	return (POLL_FAILURE);
}

void Server::closeAllConn()
{
	_condenceArrayFlag = 0;
	for (int i = 0; i < _nfds; i++)
		if (_fds[i].fd > 0)
			closeConn(i);
}

/******************************************
	Server Utils
*******************************************/
Channel *Server::findChannel(std::string name)
{
	for (size_t i(0); i < _channels.size(); ++i)
	{
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return 0;
}

User *Server::findUserByNick(std::string name)
{

	for (userMap::iterator it = _userMap.begin(); it != _userMap.end(); ++it)
		if ((it->second)->getNickName() == name)
			return it->second;
	return 0;
}

User *Server::findUserByName(std::string name)
{

	for (userMap::iterator it = _userMap.begin(); it != _userMap.end(); ++it)
		if ((it->second)->getUserName() == name)
			return it->second;
	return 0;
}

Channel *Server::addChannel(std::string name, std::string key)
{
	Channel *channel = new Channel(name, key);
	_channels.push_back(channel);
	return channel;
}

void Server::deleteChannel(std::string name)
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			delete (*it);
			_channels.erase(it);
			return ;
		}
	}
}

void Server::setName(std::string name) { _name = name; }
std::string Server::getName(void) const { return _name; }
std::string Server::getPassword(void) const { return _password; }

std::vector<Channel *> *Server::getChannels(void) { return &_channels; }

/**************************************
 * https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select
 * https://www.geeksforgeeks.org/socket-programming-cc/

 * structure of a sockadd_in *

	struct sockaddr_in {
		short            sin_family;   // e.g. AF_INET
		unsigned short   sin_port;     // e.g. htons(3490)
		struct in_addr   sin_addr;     // see struct in_addr, below
			char             sin_zero[8];  // zero this if you want 	to
			};

	struct in_addr {
		unsigned long s_addr;  // load with inet_aton()
	};

 * different type of communication *

		SOCK_STREAM: TCP(reliable, connection oriented)
		SOCK_DGRAM: UDP(unreliable, connectionless)

 * protocol : man protocol
*/

// c++ srcs/main.cpp srcs/Server.cpp incs/headers.hpp incs/Server.hpp
