/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:37:24 by lchan             #+#    #+#             */
/*   Updated: 2022/12/07 13:32:42 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "headers.hpp"
#include "Commands.hpp"

#define LOCAL_HOST "127.0.0.1"
#define SERVER_DEFAULT_PORT 8084
#define BUFFER_SIZE 1024
#define MAX_PWD 100
#define MAX_CLIENT 1000
#define TIMEOUT 201000
#define SERVER_VERSION "Ubuntu 22.04.1 LTS"
#define SERVER_NAME "<ircserv>"

#define TIMEOUT_MESS "time out - server automatic shutdown has been requested"
#define ACCEPTED "the server has accepted your connection"
#define SERVER_START_MESS "Server lauched"
#define POLL_ERR_MESS "Poll() failed"
#define _CRLF "\r\n"

enum e_socket
{
	E_SOCK_SUCCESS,
	E_SOCKET_ERR,
	E_SETSOCKOPT_ERR,
	E_SETNONBLOKING_ERR,
	E_BIND_ERR,
	E_LISTEN_ERR,
	E_ACCEPT_ERR,
};

enum e_server_status
{
	OFF_STATUS,
	ON_STATUS
};

enum e_poll_return
{
	POLL_FAILURE = -1,
	POLL_OK,
};

/*************************************************************
 * A map including an int : the fd of the socket and a pointer to the users
 * the map will be used by the server to updated its _cmdBuffer
 *************************************************************/

typedef std::map<int, User *> userMap;
typedef std::map<int, std::string> cmdMap;

class Server
{
public:
	Server();
	Server(int port, std::string pwd);
	~Server();

	void startServer();
	/* Note to delete: putting these functions here in public
	to be able to use them with Command class */
	void setName(std::string name);
	std::string getName(void) const;
	std::string getPassword(void) const;
	std::vector<Channel *> *getChannels(void);
	Channel *findChannel(std::string name);
	User *findUserByNick(std::string name);
	User *findUserByName(std::string name);
	Channel *addChannel(std::string name, std::string key);
	void deleteChannel(std::string name);

	void sendMsg(int fd, std::string &msg);
	void sendMessage(int fd, std::string id, std::string &msg);

	void sendToAllUser(std::string &msg);

	void closeFd(int targetFd);
	void closeAllConn();

private:
	int _port;
	std::string _password;
	std::string _name;
	std::string _creationTime;

	/*poll*/
	struct sockaddr_in _sockAddr;
	int _addrlen;
	int _listenSd;
	bool _status;
	bool _condenceArrayFlag;
	char _buffer[BUFFER_SIZE];
	int _opt;
	int _nfds;
	int _newSd;
	struct pollfd _fds[MAX_CLIENT];

	/*cmd*/
	userMap _userMap;
	cmdMap _cmdMap;
	cmdMap _cmdMsgMap;
	std::vector<Channel *> _channels;

	/*init the server */
	void initServer();
	int setSocket();
	int setSocketopt();
	int setNonBlocking();
	int bindSocket();
	int setListenSocket();

	/*poll management */
	int checkPollRet(int ret);
	int findReadableFd();
	int acceptNewSd();
	void readExistingFds(int fd);
	void reactToEvent(int index);
	void waitForConn();

	/*react to cmd */
	void updateServerMaps(int fd);
	void cmdMaker(int fd);
	void execCmd(int key, std::string &str);
	void handleCmd(int fd);

	/*react management Utils */
	template <typename T>
	void serverPrint(T &str) { std::cout << "[+] " << str << std::endl; }
	void closeConn(int index);
	void NarrowArray(void);
	void deleteUser(int index);
	int turnOffServer(std::string str);

	/*visual utils*/
	void PrintInfo(void);
};

#endif

// std::vector<User *>		_users;
// irssi -n Warlink -p 8084 -c localhost
