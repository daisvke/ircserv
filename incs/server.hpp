/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:37:24 by lchan             #+#    #+#             */
/*   Updated: 2022/11/29 23:08:25 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

# define LOCAL_HOST "127.0.0.1"
# define SERVER_PORT 8084
# define BUFFER_SIZE 1024
# define MAX_CLIENT 100
# define TIMEOUT 1100000
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

/*************************************************************
* A map including an int : the fd of the socket and a pointer to the users
* the map will be used by the server to updated its _cmdBuffer
*************************************************************/

typedef std::map<int, User *>	userMap;
typedef std::map<int, std::string> cmdMap;

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
		userMap				_userMap;
		cmdMap				_cmdMap;
		//std::list<struct pollfd>	_listFds;


		std::string				_password;
		//std::vector<User *>		_users;
		std::vector<Channel *>	_channels;

		/*init the server */
		void	initServer();
		int		setSocket();
		int		setSocketopt();
		int		setNonBlocking();
		int		bindSocket();
		int		setListenSocket();

		/*poll management */
		int		checkPollRet( int ret );
		int		findReadableFd();
		int		acceptNewSd();
		void	readExistingFds(int fd);
		void	reactToEvent(int index);
		void	waitForConn();

		/*react to cmd */
		void	updateServerMaps(int fd);
		void	execCmd(int fd);
		void	handleCmd(int fd);

		/*react management Utils */
		template <typename T> void	serverPrint(T & str){	std::cout << "[+] " << str << std::endl;}
		void						closeConn(int index);
		void						closeAllConn();
		int							turnOffServer(std::string str);



};

#endif

//irssi -n Warlink -p 8084 -c localhost
