/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:37:24 by lchan             #+#    #+#             */
/*   Updated: 2022/11/17 14:39:38 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

# define TIMEOUT 5000
# define TIMEOUT_MESS "time out - server automatic shutdown has been requested"
# define ACCEPTED "the server has accepted your connection"
# define SERVER_START_MESS "[+] Server lauched"

enum e_socket{
	E_SOCK_SUCCESS,
	E_SOCKET_ERR,
	E_SETSOCKOPT_ERR,
	E_SETNONBLOKING_ERR,
	E_BIND_ERR,
	E_LISTEN_ERR,
	E_ACCEPT_ERR,
};

enum e_server_status{
	SERVER_OFF,
	SERVER_ON
};

class Server{
	public :
		Server();
		Server(Server &cpy);
		~Server();

		Server &operator=(Server &rhs);

		void				startServer();

	private :
		struct sockaddr_in	_sockAddr;
		int					_addrlen;
		int					_listenSd;
		bool				_status;
		char				_buffer[BUFFER_SIZE];
		int					_opt;
		int					_nfds;
		struct pollfd		_fds[MAX_CLIENT];
		int					_pollRet;

		void				initServer();
		int					setSocket();
		int					setSocketopt();
		int					setNonBlocking();
		int					bindSocket();
		int					setListenSocket();
		void				waitForConn(); //Attendre un événement concernant un descripteur de fichier



};

#endif
