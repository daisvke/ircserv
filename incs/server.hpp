/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:37:24 by lchan             #+#    #+#             */
/*   Updated: 2022/11/15 18:34:30 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "headers.hpp"

enum e_socket{
	E_SOCK_SUCCESS,
	E_SOCKET_ERR,
	E_SETSOCKOPT_ERR,
	E_BIND_ERR,
	E_LISTEN_ERR,
	E_ACCEPT_ERR,
};

class Server{
	public :
		Server();
		Server(Server &cpy);
		~Server();

		Server &operator=(Server &rhs);


		void				initServer();
		void				waitClient(); //Attendre un événement concernant un descripteur de fichier  

	private :
		struct sockaddr_in	_sockAddr;
		int					_listenSd;
		int					_addrlen;
		char				_buffer[BUFFER_SIZE];
		int					_opt;

		struct pollfd		_pollFds[MAX_CLIENT];
		int					_pollRet;
		int					_clientsInUse;


		int					setSocket();
		int					setSocketopt();
		int					setNonBlocking();
		int					bindSocket();
		int					listenSocket();

		int					acceptClient();
};

#endif
