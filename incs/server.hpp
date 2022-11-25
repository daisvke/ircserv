/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:37:24 by lchan             #+#    #+#             */
/*   Updated: 2022/11/22 12:03:08 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

# define LOCAL_HOST "127.0.0.1"
# define SERVER_PORT 8084
# define BUFFER_SIZE 1024
# define MAX_CLIENT 100
# define TIMEOUT 11000
# define TIMEOUT_MESS "time out - server automatic shutdown has been requested"
# define ACCEPTED "the server has accepted your connection"
# define SERVER_START_MESS "Server lauched"
# define POLL_ERR_MESS "Poll() failed"

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
	OFF_STATUS,
	ON_STATUS
};

enum e_return {
	POLL_FAILURE = -1,
	POLL_OK,
};

class Server{
	public :
		Server();
		Server(Server &cpy);
		~Server();

		Server &operator=(Server &rhs);

		void				startServer();

		/* Note to delete: putting these functions here in public
		to be able to use them with Command class */
		std::string				getPassword(void) const;
		std::vector<Channel *>	getChannels(void) const;
		Channel					*findChannel(std::string name);
		User					*findUserByNick(std::string name);
		User					*findUserByName(std::string name);


	private :
		struct sockaddr_in	_sockAddr;
		int					_addrlen;
		int					_listenSd;
		bool				_status;
		char				_buffer[BUFFER_SIZE];
		int					_opt;
		int					_nfds;
		int					_newSd;
		struct pollfd		_fds[MAX_CLIENT];

		std::string				_password;
		std::vector<User *>		_users;
		std::vector<Channel *>	_channels;
		//int					_pollRet;

		void				initServer();
		int					setSocket();
		int					setSocketopt();
		int					setNonBlocking();
		int					bindSocket();
		int					setListenSocket();
		void				waitForConn(); //Attendre un événement concernant un descripteur de fichier
		int					checkPollRet( int ret );
		//void				serverPrint(const char * str);
		void				reactToEvent(int fd);
		int					findReadableFd();
		int					acceptNewSd();
		void				readExistingFds(int fd);


		void				closeConn(int index);
		int					turnOffServer(std::string str);


		template <typename T>
		void				serverPrint(T & str){	std::cout << "[+] " << str << std::endl;}
};

#endif
