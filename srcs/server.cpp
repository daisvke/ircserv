/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:20:36 by lchan             #+#    #+#             */
/*   Updated: 2022/11/17 14:36:46 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*********************************************
* 				Coplien Form
*********************************************/

Server::Server() : _addrlen(sizeof(_sockAddr)), _listenSd(-1), _status(SERVER_OFF), _opt(1), _nfds(0), _pollRet(0){

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
	_status = SERVER_ON;
	std::cout << SERVER_START_MESS << std::endl;
}


/*
	*/
void	Server::waitForConn(){
	int	ret = poll(_fds, _nfds, TIMEOUT);

	do {
		if (ret < 0)
			break ;
		else if (ret == 0)
		{
			std::cout << TIMEOUT_MESS << std::endl;
			_status = SERVER_OFF;
		}
		//checkListensd();
	}
	while (_status == SERVER_ON);
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


//  do
//   {
//     printf("Waiting on poll()...\n");

//     rc = poll(fds, nfds, timeout);
//     if (rc < 0)
//     {
//       perror("  poll() failed");
//       break;
//     }
//     if (rc == 0)
//     {
//       printf("  poll() timed out.  End program.\n");
//       break;
//     }

//     /***********************************************************/ //FIND READANLE FDS
//     /* One or more descriptors are readable.  Need to          */
//     /* determine which ones they are.                          */
//     /***********************************************************/
//     current_size = nfds;
//     for (i = 0; i < current_size; i++)
//     {
//       /*********************************************************/
//       /* Loop through to find the descriptors that returned    */
//       /* POLLIN and determine whether it's the listening       */
//       /* or the active connection.                             */
//       /*********************************************************/
//       if(fds[i].revents == 0)
//         continue;




//       /*********************************************************/
//       /* If revents is not POLLIN, it's an unexpected result,  */
//       /* log and end the server.                               */
//       /*********************************************************/
//       if(fds[i].revents != POLLIN)
//       {
//         printf("  Error! revents = %d\n", fds[i].revents);
//         end_server = TRUE;
//         break;

//       }




//       if (fds[i].fd == listen_sd)
//       {
//         /*******************************************************/
//         /* Listening descriptor is readable.                   */
//         /*******************************************************/
//         printf("  Listening socket is readable\n");

//         /*******************************************************/
//         /* Accept all incoming connections that are            */
//         /* queued up on the listening socket before we         */
//         /* loop back and call poll again.                      */
//         /*******************************************************/
//         do
//         {
//           /*****************************************************/
//           /* Accept each incoming connection. If               */
//           /* accept fails with EWOULDBLOCK, then we            */
//           /* have accepted all of them. Any other              */
//           /* failure on accept will cause us to end the        */
//           /* server.                                           */
//           /*****************************************************/
//           new_sd = accept(listen_sd, NULL, NULL);
//           if (new_sd < 0)
//           {
//             if (errno != EWOULDBLOCK)
//             {
//               perror("  accept() failed");
//               end_server = TRUE;
//             }
//             break;
//           }

//           /*****************************************************/
//           /* Add the new incoming connection to the            */
//           /* pollfd structure                                  */
//           /*****************************************************/
//           printf("  New incoming connection - %d\n", new_sd);
//           fds[nfds].fd = new_sd;
//           fds[nfds].events = POLLIN;
//           nfds++;

//           /*****************************************************/
//           /* Loop back up and accept another incoming          */
//           /* connection                                        */
//           /*****************************************************/
//         } while (new_sd != -1);
//       }

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
