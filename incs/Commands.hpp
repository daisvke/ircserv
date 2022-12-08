/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:10 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/07 11:31:01 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <vector>
# include <algorithm>
# include <map>

# include "Utils.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "server.hpp"

enum	e_isOper { _ISNOTOPER, _ISOPER };

# define _NETWORKNAME		"IRC-LCHAN-DTANIGAWcom"
# define _SERVERVERSION		"0.0.1"
# define _USERMODES			"ovinsk"
# define _CHANMODES			"ovtimnsplk"
# define _CHAN_PARAM_MODES	"ovlk"

// authentification
# define _RPL_WELCOME(nick, user)		"001 " + nick + " :Welcome to the " + _NETWORKNAME + " Network, " + user
# define _RPL_YOURHOST(nick, server)	"002 " + nick + " :Your host is " + server + ", running version " + _SERVERVERSION
# define _RPL_CREATED(nick, datetime)	"003 " + nick + " :This server was created " + datetime
# define _RPL_MYINFO(nick, server)		"004 " + nick + " " + server + " " + _SERVERVERSION + " " + _USERMODES \
	+ " " + _CHANMODES + " " + _CHAN_PARAM_MODES
// commands
# define _ERR_UNKNOWNCOMMAND(nick, cmd)		"421 " + nick + " " + cmd
# define _ERR_NEEDMOREPARAMS(nick, cmd)		"461 " + nick + " " + cmd
// nick
# define _RPL_CURRENTNICK(nick)				"Your nickname is " + nick
# define _RPL_NICKSUCCESS(nick)				"You're now known as " + nick
# define _ERR_NONICKNAMEGIVEN(nick)			"431 " + nick
# define _ERR_NICKNAMEINUSE(nick)			"433 " + nick + nick
// user
# define _ERR_ALREADYREGISTRED(user)		user + " already registered"
// whois
# define _RPL_WHOISREGNICK(nick)			"307 " + nick + " :has identified for this nick"
# define _RPL_ENDOFWHOIS(nick)					"318 " + nick + " :End of /WHOIS list"
// oper
# define _RPL_YOUREOPER						"You are oper"
# define _ERR_PASSWDMISMATCH(nick)			"464 " + nick
// join
# define _ERR_BADCHANNELKEY(chan)			"Wrong key for " + chan
# define _ERR_CHANNELISFULL(chan)			"Channel " + chan + " is full"
# define _ERR_INVITEONLYCHAN(chan)			"Channel " + chan + " is invite only"
# define _RPL_TOPIC(nick, chan, topic)		"332 " + nick + " " + chan + " :" + topic
# define _RPL_NAMREPLY(nick, name, symbol, chan, prefix) "353 " + nick + " " + symbol + " " + chan + prefix + name
# define _RPL_ENDOFNAMES(nick, chan)		"366 " + nick + " " + chan
// part
# define _ERR_NOSUCHCHANNEL(nick, chan)		"403 " + nick + " " + chan
# define _ERR_NOSUCHCHANIMIT(nick, chan)	"403 " + nick + " " + chan
# define _ERR_NOTONCHANNEL(nick, chan)		"442 " + nick + " " + nick + " is not on channel " + chan
// ping
# define _ERR_NOSUCHSERVER(server)			server + " :No such server"
// privmsg
# define _ERR_NOSUCHNICK(nick)				"401 " + nick
# define _ERR_CANNOTSENDTOCHAN(nick, chan)	"404 " + nick + " " + chan
# define _ERR_CHANOPRIVSNEEDED(nick)		"482 " + nick
// invite
# define _ERR_USERONCHANNEL(nick, chan)		"443 " + nick + " Already on channel " + chan
// mode
# define _RPL_UMODEIS(nick, modes)			"221 " + nick + " " + modes
# define _RPL_CHANNELMODEIS(nick, chan, modes, args)	"324 " + nick + " " + chan + " " + modes + " " + args
// topic
# define _RPL_TOPICWHOTIME(nick, chan, setat)	"333 " + nick + " " + chan + " " + nick + " " + setat
# define _RPL_TOPICCHANGESUCCESS(nick, chan, topic)	nick + " changed the topic of " + chan + " to: " + topic
// list
# define _RPL_LISTSTART(nick)				"321 " + nick + " Channel :Users Name"
# define _RPL_LIST(nick, chan, clientCount, topic)	"322 " + nick + " " + chan + " " + clientCount + " :" + topic
# define _RPL_LISTEND(nick)					"323 " + nick + " :End of /LIST"


class Server ;

class Commands
{
	typedef std::map<std::string, void(Commands::*)(void)> cmdMap;

	public:

		Commands(Server *server, User *user, std::string &str);
		~Commands();


	private:

		void	pass(void);
		void	nick(void);
		void	user(void);
		void	whois(void);
		void	who(void);
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

		enum	e_isPriv { _NOT_PRIV, _PRIV };
		void	broadcastToChannel(Channel *channel, std::string msg, bool isPriv);


		Server						*_server;
		User						*_user;
		std::vector<std::string>	_params;
		cmdMap						_cmdMap;
};

#endif