	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:20:36 by lchan             #+#    #+#             */
/*   Updated: 2022/11/08 16:11:54 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

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
}

Server	&Server::operator=(Server &rhs){

	_sockAddr = rhs._sockAddr;
	_socket = rhs._socket;
	_addrlen = rhs._addrlen;
	for (int i = 0; i < BUFFER_SIZE; i++)
		rhs._buffer[i] = _buffer[i];
	_opt = rhs._opt;
	return (*this);
}

		// throw std::invalid_argument("Server socket fail");
		// throw std::invalid_argument("Server setsockopt fail");
		// throw std::invalid_argument("Server bind fail");



/*********************************************
 * 				initServer
 *********************************************/

int		Server::setSocket()
{
	if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
		return (SOCKET_ERR);
	else
		std::cout << "1. socket success" << std::endl;
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	_sockAddr.sin_port = htons(PORT);
	return (SOCK_SUCCESS);
}

int		Server::setSocketopt()
{
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) == SOCKET_ERROR)
		return (SETSOCKOPT_ERR);
	std::cout << "2. socketopt success" << std::endl;
	return (SOCK_SUCCESS);
}

int		Server::bindSocket()
{
	if ((bind(_socket, (sockaddr *) &_sockAddr, _addrlen)) == SOCKET_ERROR)
		return (BIND_ERR);
	std::cout << "3. bind success" << std::endl;
	return (SOCK_SUCCESS);
}

int		Server::listenSocket()
{
	if (listen(_socket, MAX_CLIENT) == SOCKET_ERROR )
		return (LISTEN_ERR);
	std::cout << "4. listening" << std::endl;
	return (SOCK_SUCCESS);
}

void	Server::initServer()
{
	std::string names[5] = { "socket", "socketopt", "bind", "listen", "accept"};
	int	((Server::*funkTab[4])()) = {
		&Server::setSocket,
		&Server::setSocketopt,
		&Server::bindSocket,
		&Server::listenSocket
		};

	for (int i = 0; i < 4; i++)
		if ((this->*funkTab[i])() == SOCKET_ERROR)
			throw std::invalid_argument(names[i] + " error");
	std::cout << SERVER_START_MESS << std::endl; 
}



// void	Server::listen(){
// 	if (listen(_socket, 5) == SOCKET_ERROR )
// 		throw std::invalid_argument("Server listen fail");
// 	else
// 		std::cout << "4. listening" << std::endl;


// 	clientSocket = accept(_socket, (sockaddr *)&_sockAddr, (socklen_t*) &_addrlen);
// 	if (clientSocket == SOCKET_ERROR)
// 		throw std::invalid_argument("Server accept fail");
// 	else
// 		std::cout << "5. New socket accepted" << std::endl;

// 	// if ((clientSocket = accept(_socket, (sockaddr *)&ServerAddr, (socklen_t*) &_addrlen))== SOCKET_ERROR)
// 	// 	throw std::invalid_argument("Server accept fail");

// 	sendRet = read(clientSocket, buffer, sizeof(buffer));

// 	send(clientSocket, ACCEPTED, strlen(ACCEPTED), 0);
// 	std::cout << "Server buffer contains: [" << buffer << "]" << "[" << sendRet << "]" << std::endl;

// 	std::cout << "closing Server" << std::endl;
// }

void	server::waitClient(){

	_pollFds[0].fd = _socket;
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


/*
void server(){

	struct sockaddr_in	serverAddr;
	int					serverSocket;
	int					clientSocket;
	int					sendRet;
	int					addrlen = sizeof(serverAddr);
	int					opt = 1;
	char				buffer[BUFFER_SIZE] = { 0 };

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
		throw std::invalid_argument("server socket fail");
	else
		std::cout << "1. socket success" << std::endl;

 	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		throw std::invalid_argument("server setsockopt fail");
	else
		std::cout << "2. socketopt success" << std::endl;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	serverAddr.sin_port = htons(PORT);

	if (bind(serverSocket, (sockaddr *) &serverAddr, addrlen) == SOCKET_ERROR)
		throw std::invalid_argument("server bind fail");
	else
		std::cout << "3. bind success" << std::endl;


	if (listen(serverSocket, 5) == SOCKET_ERROR )
		throw std::invalid_argument("server listen fail");
	else
		std::cout << "4. listening" << std::endl;

	clientSocket = accept(serverSocket, (sockaddr *)&serverAddr, (socklen_t*) &addrlen);
	if (clientSocket == SOCKET_ERROR)
		throw std::invalid_argument("server accept fail");
	else
		std::cout << "5. New socket accepted" << std::endl;

	// if ((clientSocket = accept(serverSocket, (sockaddr *)&serverAddr, (socklen_t*) &addrlen))== SOCKET_ERROR)
	// 	throw std::invalid_argument("server accept fail");

	sendRet = read(clientSocket, buffer, sizeof(buffer));

	send(clientSocket, ACCEPTED, strlen(ACCEPTED), 0);
	std::cout << "server buffer contains: [" << buffer << "]" << "[" << sendRet << "]" << std::endl;

	std::cout << "closing server" << std::endl;
}


int main(){
	try {
		server();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
}
*/
/*
c++ -std=c++98 server.cpp -o server
*/