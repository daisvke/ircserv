/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:20:36 by lchan             #+#    #+#             */
/*   Updated: 2022/11/18 15:41:16 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*********************************************
* 				Coplien Form
*********************************************/

Server::Server() : _addrlen(sizeof(_sockAddr)), _listenSd(-1), _status(OFF_STATUS), _opt(1), _nfds(0), _newSd(0)/*, _pollRet(0)*/{

	IrcMemset((void *)_buffer, 0, sizeof(_buffer));
	IrcMemset((void *)_fds, 0, sizeof(_fds));
	std::cout << "Server constructor called" << std::endl;
}

Server::Server(Server &cpy) {
	*this = cpy;
}

Server::~Server(){

	if (_nfds){
		for (int i = 0; i < _nfds; i++)  // Clean up all of the sockets that are open
			if(_fds[i].fd >= 0)
				close(_fds[i].fd);
	}
	else if (_listenSd >= 0)
			close (_listenSd);
	std::cout << "Server destructor called" << std::endl;
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
	* Set socket to be nonblocking. (FIONDBIO )
	* 	--> (similar to O_NONBLOCK flag with the fcntl subroutine)
	* All incoming connections will inherit that state from listening socket
	*************************************************************/
int		Server::setNonBlocking()
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
int		Server::bindSocket()
{
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	_sockAddr.sin_port = htons(SERVER_PORT);
	if ((bind(_listenSd, (sockaddr *) &_sockAddr, _addrlen)) == ERROR)
		return (E_BIND_ERR);
	std::cout << "4. bind success" << std::endl;
	return (E_SOCK_SUCCESS);
}

	/*************************************************************
	* Set the listen back log and initial listening socket
	*************************************************************/
int		Server::setListenSocket()
{
	if (listen(_listenSd, MAX_CLIENT) == ERROR )
		return (E_LISTEN_ERR);
	_fds[0].fd = _listenSd;
	_fds[0].events = POLLIN;
	std::cout << "5. listening" << std::endl;
		return (E_SOCK_SUCCESS);
}

	/*************************************************************
	* Server initialisation function
	*************************************************************/
void	Server::initServer()
{
	std::string names[5] = {
		"socket",
		"socketopt",
		"setNonBlocking"
		"bind",
		"listen",
	};
	int	(Server::*funkTab[5])() = {
		&Server::setSocket,
		&Server::setSocketopt,
		&Server::setNonBlocking,
		&Server::bindSocket,
		&Server::setListenSocket
	};

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
int	Server::checkPollRet( int ret ){

	switch (ret){
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

int	Server::findReadableFd(){

	int	i;

	std::cout << "inside find readable fd" << std::endl;

	for (i = 0; i < _nfds; i++)
		std::cout << "found : " << _fds[i].fd << " _listenSd = " << _listenSd << std::endl;
	for (i = 0; i < _nfds;  i++){
		if (_fds[i].revents == 0)
			continue;						//do next loop,
		if (_fds[i].revents != POLLIN)		//If revents is not POLLIN it's an unexpected result;
		{
			_status = OFF_STATUS;
			return (POLL_FAILURE);
		}
		else
		{
			std::cout << "returning : " << i << std::endl;
			return (i);
		}
	}
	return (POLL_FAILURE);
}

	/****************************************************************************
	Accept all incoming connections that are queued up on the listening socket
	before we loop back and call poll again.
	*****************************************************************************/

int	Server::acceptNewSd(){
	do{
		_newSd = accept(_listenSd, NULL, NULL);	//ckeck on man to see if other argument are necessary or not
		if (_newSd < 0){
			if (errno != EWOULDBLOCK)
				return (turnOffServer("accept() failed"));
			break ;
		}
		printf("[DEBUG_MESS] acceptNewSd : a new connection has been made ! fd = %d \n", _newSd);
		_fds[_nfds].fd = _newSd;				//	check if it is necessary to find a way to use the new reusable fd. Maybe accept can do it automatically ?
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
	while (_newSd != -1);
	return (POLL_OK);
}

/****************************************************************************
 * if poll does not fail, it's either :
	-> _listenSd (listen socket descriptor) is readable (a new connection is incomming)
	-> _fds[i] (a pollfd) is readable
 ****************************************************************************/
void	Server::reactToEvent(int fd){

	if (fd == POLL_FAILURE)
		return ;
	else if (_fds[fd].fd == _listenSd)
		acceptNewSd();
	else
		readFds();
}

void	Server::readFds(){
	serverPrint("reactToEvent : fd is readable, a new message has been received");
	int	closedFlag = OFF_STATUS;

	do{
		//recv function
		//need to link on channel ??
	}
	while()

}

//       /*********************************************************/
//       /* This is not the listening socket, therefore an        */
//       /* existing connection must be readable                  */
//       /*********************************************************/
//       else
//       {
//         printf("  Descriptor %d is readable\n", fds[i].fd);
//         close_conn = FALSE;
//         /*******************************************************/
//         /* Receive all incoming data on this socket            */
//         /* before we loop back and call poll again.            */
//         /*******************************************************/

//         do
//         {
//           /*****************************************************/
//           /* Receive data on this connection until the         */
//           /* recv fails with EWOULDBLOCK. If any other         */
//           /* failure occurs, we will close the                 */
//           /* connection.                                       */
//           /*****************************************************/
//           rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
//           if (rc < 0)
//           {
//             if (errno != EWOULDBLOCK)
//             {
//               perror("  recv() failed");
//               close_conn = TRUE;
//             }
//             break;
//           }

//           /*****************************************************/
//           /* Check to see if the connection has been           */
//           /* closed by the client                              */
//           /*****************************************************/
//           if (rc == 0)
//           {
//             printf("  Connection closed\n");
//             close_conn = TRUE;
//             break;
//           }

//           /*****************************************************/
//           /* Data was received                                 */
//           /*****************************************************/
//           len = rc;
//           printf("  %d bytes received\n", len);

//           /*****************************************************/
//           /* Echo the data back to the client                  */
//           /*****************************************************/
//           rc = send(fds[i].fd, buffer, len, 0);
//           if (rc < 0)
//           {
//             perror("  send() failed");
//             close_conn = TRUE;
//             break;
//           }

//         } while(TRUE);

//         /*******************************************************/
//         /* If the close_conn flag was turned on, we need       */
//         /* to clean up this active connection. This            */
//         /* clean up process includes removing the              */
//         /* descriptor.                                         */
//         /*******************************************************/
//         if (close_conn)
//         {
//           close(fds[i].fd);
//           fds[i].fd = -1;
//           compress_array = TRUE;
//         }


//       }  /* End of existing connection is readable             */
//     } /* End of loop through pollable descriptors              */



//     /***********************************************************/
//     /* If the compress_array flag was turned on, we need       */
//     /* to squeeze together the array and decrement the number  */
//     /* of file descriptors. We do not need to move back the    */
//     /* events and revents fields because the events will always*/
//     /* be POLLIN in this case, and revents is output.          */
//     /***********************************************************/
//     if (compress_array)
//     {
//       compress_array = FALSE;
//       for (i = 0; i < nfds; i++)
//       {
//         if (fds[i].fd == -1)
//         {
//           for(j = i; j < nfds; j++)
//           {
//             fds[j].fd = fds[j+1].fd;
//           }
//           i--;
//           nfds--;
//         }
//       }
//     }





//   } while (end_server == FALSE); /* End of serving running.    */





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


/*******************************************
* poll is blocking until
	-> it catchs an event(new fds or listening socket is readable)
	-> it fails
	-> it times out
* poll has to be in the loop (recalled each time). it actes like a reset
********************************************/
void	Server::waitForConn(){

	do {
		if (checkPollRet(poll(_fds, _nfds, TIMEOUT)) == POLL_FAILURE)
			break ;
		else
			reactToEvent(findReadableFd());
	}
	while (_status == ON_STATUS);
}

void	Server::startServer(){
	try {
			initServer();
			waitForConn();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
}

/******************************************
	Server Utils
*******************************************/
/*
void	Server::serverPrint(std::string str){

	std::cout << "[+] " << str << std::endl;
}*/

int		Server::turnOffServer(std::string str){
	serverPrint(str);
	_status = OFF_STATUS;
	return (POLL_FAILURE);
}




