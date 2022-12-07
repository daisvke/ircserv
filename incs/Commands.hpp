/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:10 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/02 21:51:38 by lchan            ###   ########.fr       */
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

enum	e_isOper { _ISNOTOPER, _ISOPER };

# define NICK		"NICK"
# define USER		"USER"
# define OPER		"OPER"
# define QUIT		"QUIT"
# define JOIN		"JOIN"
# define PART		"PART"
# define MODE		"MODE"
# define TOPIC		"TOPIC"
# define NAMES		"NAMES"
# define LIST		"LIST"
# define INVITE		"INVITE"
# define KICK		"KICK"
# define KILL		"KILL"
# define PING		"PING"
# define PONG		"PONG"

# define _NETWORKNAME		"IRC-LCHAN-DTANIGAWcom"
# define _SERVERVERSION		"0.0.1"
# define _USERMODES			"ovinsk"
# define _CHANMODES			"ovtimnsplk"//b ?
# define _CHAN_PARAM_MODES	"ovlk"

// authentification
# define _RPL_WELCOME(nick, user)		"001 " + nick + " :Welcome to the " + _NETWORKNAME + " Network, " + user
# define _RPL_YOURHOST(nick, server)	"002 " + nick + " :Your host is " + server + ", running version " + _SERVERVERSION
# define _RPL_CREATED(nick, datetime)	"003 " + nick + " :This server was created " + datetime
# define _RPL_MYINFO(nick, server)		"004 " + nick + " " + server + " " + _SERVERVERSION + " " + _USERMODES \
	+ " " + _CHANMODES + " " + _CHAN_PARAM_MODES

// commands
# define _ERR_NOSUCHCMD(cmd)				"Unknown command: " + cmd
# define _ERR_NEEDMOREPARAMS				"Not enough parameters given"

// nick
# define _RPL_CURRENTNICK(nick)				"Your nickname is " + nick
# define _RPL_NICKSUCCESS(nick)				"You're now known as " + nick
# define _ERR_NONICKNAMEGIVEN(nick)				"431 " + nick + "No nickname given"
# define _ERR_NICKNAMEINUSE(nick)			"Nick " + nick + " is already in use"
// user
# define _ERR_ALREADYREGISTRED(user)		user + " already registered"
// whois
# define _RPL_WHOISREGNICK(nick)			"307 " + nick + " :has identified for this nick"
# define _RPL_ENDOFWHOIS(nick)					"318 " + nick + " :End of /WHOIS list"
// oper
# define _RPL_YOUREOPER						"You are oper"
# define _ERR_PASSWDMISMATCH(nick)			"464 " + nick + " :password incorrect"
// join	
# define _ERR_BADCHANNELKEY(chan)			"Wrong key for " + chan
# define _ERR_CHANNELISFULL(chan)			"Channel " + chan + " is full"
# define _ERR_INVITEONLYCHAN(chan)			"Channel " + chan + " is invite only"
# define _RPL_TOPIC(nick, chan, topic)		"332 " + nick + " #" + chan + " :" + topic
# define _RPL_NAMREPLY(nick, name, symbol, chan, prefix) "353 " + nick + " " + symbol + " " + chan + prefix + name
# define _RPL_ENDOFNAMES(nick, chan)		"366 " + nick + " " + chan + " :End of /NAMES list"
// part
# define _ERR_NOSUCHCHANNEL(nick, chan)		"403 " + nick + " " + chan + " No such channel"
# define _ERR_NOTONCHANNEL(nick, chan)		"442 " + nick + " " + nick + " is not on channel #" + chan
// ping
# define _ERR_NOSUCHSERVER(server)			server + " :No such server"
// privmsg
# define _ERR_NOSUCHNICK(nick)				"401 " + nick + " No such nick"
# define _ERR_CANNOTSENDTOCHAN(nick, chan)	"404 " + nick + " Cannot send to channel '" + chan + "'"
# define _ERR_CHANOPRIVSNEEDED(nick)		"482 " + nick + " Need to be operator"
// invite
# define _ERR_USERONCHANNEL(nick, chan)			"443 " + nick + " Already on channel " + chan


class Server ;

class Commands
{
	typedef std::map<std::string, void(Commands::*)(void)> cmdMap;

	public:

		Commands(Server *server, User *user, std::string &str);
		~Commands();

	public:

		// getter
		std::string	&getRpl();

	private:

		void	pass(void);
		void	nick(void);
		void	user(void);
		void	whois(void);
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
		void	privmsg(bool isNoticeCmd);
		void	kill(void);
		void	ping(void);
		void	pong(void);

		void	setupMap();
		void	routeCmd();
		void	registerClient(void);
		void	sendMsgToChan(Channel *channel, std::string &msg);


		Server						*_server;
		User						*_user;
		std::vector<std::string>	_params;
		cmdMap						_cmdMap;

		std::string					_rpl;
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
