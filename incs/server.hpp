/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:37:24 by lchan             #+#    #+#             */
/*   Updated: 2022/11/08 16:03:10 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "headers.hpp"

enum e_socket{
	SOCK_SUCCESS,
	SOCKET_ERR,
	SETSOCKOPT_ERR,
	BIND_ERR,
	LISTEN_ERR,
	ACCEPT_ERR,
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
		int					_socket;
		int					_addrlen;
		char				_buffer[BUFFER_SIZE];
		int					_opt;

		struct pollfd		_pollFds[MAX_CLIENT];
		int					_pollRet;
		int					_clientsInUse;


		int					setSocket();
		int					setSocketopt();
		int					bindSocket();
		int					listenSocket();

		int					acceptClient();
};

#endif
