/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:10 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/01 15:53:51 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <vector>
# include <algorithm>
# include <map>

# include "User.hpp"
# include "Channel.hpp"
# include "server.hpp"

# define NICK	"NICK"
# define USER	"USER"
# define OPER	"OPER"
# define QUIT	"QUIT"
# define JOIN	"JOIN"
# define PART	"PART"
# define MODE	"MODE"
# define TOPIC	"TOPIC"
# define NAMES	"NAMES"
# define LIST	"LIST"
# define INVITE	"INVITE"
# define KICK	"KICK"
# define KILL	"KILL"

class Server ;

class Commands
{
	typedef std::map<std::string, void(Commands::*)(void)> cmdMap;

	public:

		Commands(Server *server, User *user, std::string &str);
		~Commands();

	private:

		void	nick(void);
		void	user(void);
		void	oper(void);
		void	quit(void);
		void	join(void);
		void	part(void);
		void	mode(void);
		void	topic(void);
		void	names(void);
		void	list(void);
		void	invite(void);
		void	kick(void);
		void	kill(void);

		void	setupMap();
		void	routeCmd();

		Server						*_server;
		User						*_user;
		std::vector<std::string>	_params;
		cmdMap						_cmdMap;


};

#endif



/*************/
/* to delete */
/*************/
/*

	// Commands(Server *server, User *user, t_message msg);

	t_message	_message;
	std::map<std::string, f>	_cmdMap;
	std::map<std::string, f> _cmdMap;
	std::map<std::string, void(Commands::*)(void)>   _cmdMap;

enum	e_commands
{
	_NICK,
	_USER,
	_OPER,
	_QUIT,
	_JOIN,
	_PART,
	_MODE,
	_TOPIC,
	_NAMES,
	_LIST,
	_INVITE,
	_KICK,
	_KILL
};
*/
/*************************************************************
* Contains the message sent by the user.
* The 'params' variable contains the whole input, including
* 	the command and the parameters
*************************************************************/
// typedef struct s_message
// {
// 	int							cmd;
// 	std::vector<std::string>	params;
// }	t_message;
