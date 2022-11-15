/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:20:36 by lchan             #+#    #+#             */
/*   Updated: 2022/11/15 18:40:56 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/server.hpp"

				/*********************************************
				* 				Coplien Form
				*********************************************/

Server::Server() : _addrlen(sizeof(_sockAddr)), _opt(1), _clientsInUse(0), _pollRet(0){

	std::cout << "Server constructor called" << std::endl;
	memset((void *)_buffer, 0, BUFFER_SIZE);
	// for(int i = 0; i < BUFFER_SIZE; i++)
	// 	_buffer[i] = 0;
}

Server::Server(Server &cpy) {
	*this = cpy;
}

Server::~Server(){

	std::cout << "Server destructor called" << std::endl;
	if (_listenSd)
		close (_listenSd);
}

Server	&Server::operator=(Server &rhs){

	_sockAddr = rhs._sockAddr;
	_listenSd = rhs._listenSd;
	_addrlen = rhs._addrlen;
	for (int i = 0; i < BUFFER_SIZE; i++)
		rhs._buffer[i] = _buffer[i];
	_opt = rhs._opt;
	return (*this);
}

				/*********************************************
				 * 				initServer
				 *********************************************/

/*************************************************************
 * Create stream socket to receive incoming connections
 * Set 
 *************************************************************/
int		Server::setSocket()
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
int		Server::setSocketopt()
{
	if (setsockopt(_listenSd, SOL_SOCKET, SO_REUSEADDR 
		| SO_REUSEPORT, &_opt, sizeof(_opt)) == ERROR)
		return (E_SETSOCKOPT_ERR);
	std::cout << "2. socketopt success" << std::endl;
	return (E_SOCK_SUCCESS);
}

/*************************************************************
 * Set socket to be nonblocking. 
 * All incoming connections will inherit that state from listening socket 
 *	--> will also be nonblocking since
 *************************************************************/
// int		Server::setNonBlocking()
// {
// 	int ret = ioctl(_listenSd, FIONBIO, (char *)&on);
//   	if (ret < 0)
//   		return (E_SETSOCKOPT_ERR);
// 	return (E_SOCK_SUCCESS); 
// }

/*************************************************************
 * Bind the socket
 *************************************************************/
int		Server::bindSocket()
{
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	_sockAddr.sin_port = htons(SERVER_PORT);
	if ((bind(_listenSd, (sockaddr *) &_sockAddr, _addrlen)) == ERROR)
		return (E_BIND_ERR);
	std::cout << "3. bind success" << std::endl;
	return (E_SOCK_SUCCESS);
}

/*************************************************************
 * Set the listen back log 
 *************************************************************/
int		Server::listenSocket()
{
	if (listen(_listenSd, MAX_CLIENT) == ERROR )
		return (E_LISTEN_ERR);
	std::cout << "4. listening" << std::endl;
	return (E_SOCK_SUCCESS);
}

void	Server::initServer()
{
	std::string names[5] = { 
		"socket", 
		"socketopt", 
		"bind", 
		"listen", 
		"accept"
	};
	int	((Server::*funkTab[4])()) = {
		&Server::setSocket,
		&Server::setSocketopt,
		&Server::bindSocket,
		&Server::listenSocket
	};

	for (int i = 0; i < 4; i++)
		if ((this->*funkTab[i])() == ERROR)
			throw std::invalid_argument(names[i] + " error");
	std::cout << SERVER_START_MESS << std::endl; 
}


// 	clientSocket = accept(_listenSd, (sockaddr *)&_sockAddr, (socklen_t*) &_addrlen);
// 	if (clientSocket == ERROR)
// 		throw std::invalid_argument("Server accept fail");
// 	else
// 		std::cout << "5. New socket accepted" << std::endl;

// 	// if ((clientSocket = accept(_listenSd, (sockaddr *)&ServerAddr, (socklen_t*) &_addrlen))== E_listeningERROR)
// 	// 	throw std::invalid_argument("Server accept fail");

// 	sendRet = read(clientSocket, buffer, sizeof(buffer));

// 	send(clientSocket, ACCEPTED, strlen(ACCEPTED), 0);
// 	std::cout << "Server buffer contains: [" << buffer << "]" << "[" << sendRet << "]" << std::endl;

// 	std::cout << "closing Server" << std::endl;
// }


/*
void	server::waitClient(){

	_pollFds[0].fd = _listenSd;
	_pollFds[0].event = POLLIN;

	while (1)
	{
		_pollRet = poll(_pollFds, _clientsInUse + 1, POLL_DELAY);
		if (_pollRet > 0)
		{
			return;
		}
	}
}
*/



// int main(){
// 	Server	Server;
// 	try {
// 			Server.initServer();
// 	}
// 	catch (std::exception &e){
// 		std::cout << e.what() << std::endl;
// 	}

// }




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

//c++ srcs/main.cpp srcs/server.cpp incs/headers.hpp incs/server.hpp