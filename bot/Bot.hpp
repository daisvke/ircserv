/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 13:53:21 by lchan             #+#    #+#             */
/*   Updated: 2022/12/10 01:03:51 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <exception>
# include <bits/stdc++.h>
# include <vector>
# include <map>
# include <vector>
# include <algorithm>
# include <map>

# define LOCAL_HOST				"127.0.0.1"
# define MAX_PWD				100
# define BOTNAME				"ClapTrap"
# define CHAN_NAME				"#botchan"
# define BOT_CONN_CAP			"CAP LS \r\n"
# define BOT_CONN_PASS(pwd)		"PASS " + pwd + " \r\n"
# define BOT_CONN_NICK			"NICK ClapTrap \r\n"
# define BOT_CONN_USER			"USER BonusBot BonusBot localhost :LBot \r\n"
# define BOT_JOIN_CHAN(chan)	"JOIN " + chan + "\r\n"

enum e_bot_error {BOT_ERROR = -1, BOT_SUCESS};
enum e_bot_status{OFF_STATUS, ON_STATUS};

class Bot
{
	typedef std::map<int, std::string> botMap;

	public:
		Bot(int port, std::string pwd);
		~Bot();
		void		botExec();

	private :
		int					_port;
		std::string			_pwd;
		std::string			_chanName;
		bool				_status;
		int					_flag;
		int					_rplIndex;
		//setup Buffer
		struct sockaddr_in	_addrBot;
		int					_listenBSd;
		char				_buffer[1024];
		std::string					_strBuffer;
		std::vector<std::string>	_splitBuffer;
		botMap						_botMap;
		std::string					_rplBuffer;

		void	setupBot();
		void	setBotMap();
		void	connectToServer();
		void	authentification();
		void	creatChannel();

		void	readBuffer();
		void	concatRecv();
		void	waitForCompleteRecv();

		void	checkServerRpl();
		void	setRplBuffer();
		void	sendRpl();
};

#endif
