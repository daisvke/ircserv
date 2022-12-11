/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:10 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/09 20:39:51 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <algorithm>
#include <map>

#include "main.hpp"
#include "Utils.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// authentification
#define _RPL_WELCOME(nick, user) "001 " + nick + " :Welcome to the " + _NETWORKNAME + " Network, " + user
#define _RPL_YOURHOST(nick, server) "002 " + nick + " :Your host is " + server + ", running version " + _SERVERVERSION
#define _RPL_CREATED(nick, datetime) "003 " + nick + " :This server was created " + datetime
#define _RPL_MYINFO(nick, server) "004 " + nick + " " + server + " " + _SERVERVERSION + " " + _USERMODES + " " + _CHANMODES + " " + _CHAN_PARAM_MODES
// commands
#define _ERR_UNKNOWNCOMMAND(nick, cmd) "421 " + nick + " " + cmd + " :Unknown command"
#define _ERR_NEEDMOREPARAMS(nick, cmd) "461 " + nick + " " + cmd + " :Not enough parameters"
// nick
#define _RPL_CURRENTNICK(nick) "Your nickname is " + nick
#define _RPL_NICKSUCCESS(nick) "You're now known as " + nick
#define _ERR_NONICKNAMEGIVEN "431"
#define _ERR_ERRONEUSNICKNAME(nick) "432 " + nick + " :Erroneus nickname"
#define _ERR_NICKNAMEINUSE(nick) "433 " + nick + " " + nick + " :Nickname is already in use"
// user
#define _ERR_ALREADYREGISTRED(user) user + " already registered"
// whois
#define _RPL_WHOISREGNICK(nick) "307 " + nick + " " + nick
#define _RPL_WHOISUSER(nick, user, host, real) "311 " + nick + " " + user + " " + host + " * :" + real
#define _RPL_WHOISSERVER(nick, server) "312 " + nick + " " + nick + " " + server + " :" + _SERVER_INFO
#define _RPL_WHOISOPERATOR(nick) "313 " + nick + " " + nick + " :is an IRC operator"
#define _RPL_ENDOFWHOIS(nick) "318 " + nick + " :End of /WHOIS list"
// who
#define _RPL_WHOREPLY(nick, chan, user, host, server, flags, hopcount, real) \
	"352 " + nick + " " + chan + " " + user + " " + host + " " + server + " " + nick + " " + flags + " :" + hopcount + " " + real
#define _RPL_ENDOFWHO(nick) "315 " + nick + " :End of /WHO list"
// oper
#define _RPL_YOUREOPER(nick) "381 " + nick + " :You are now an IRC operator"
#define _ERR_PASSWDMISMATCH(nick) "464 " + nick + " :Password incorrect"
// join
#define _ERR_INVITEONLYCHAN(nick, chan) "473 " + nick + " " + chan + " :Cannot join channel (+i)"
#define _ERR_BADCHANNELKEY(nick, chan) "475 " + nick + " " + chan + " :Cannot join channel (+k)"
#define _ERR_CHANNELISFULL(nick, chan) "471 " + nick + " " + chan + " :Cannot join channel (+l)"
#define _RPL_TOPIC(nick, chan, topic) "332 " + nick + " " + chan + " :" + topic
#define _RPL_NAMREPLY(nick, name, symbol, chan, prefix) "353 " + nick + " " + symbol + " " + chan + " " + prefix + name
#define _RPL_ENDOFNAMES(nick, chan) "366 " + nick + " " + chan
// part
#define _ERR_NOSUCHCHANNEL(nick, chan) "403 " + nick + " " + chan
#define _ERR_NOSUCHCHANIMIT(nick, chan) "403 " + nick + " " + chan
#define _ERR_NOTONCHANNEL(nick, chan) "442 " + nick + " " + chan + " :You're not on that channel"
// ping
#define _ERR_NOSUCHSERVER(nick, server) "402 " + nick + " " + server + " :No such server"
// privmsg
#define _ERR_NOSUCHNICK(nick) "401 " + nick + " " + nick + " :No such nick/channel"
#define _ERR_CANNOTSENDTOCHAN(nick, chan) "404 " + nick + " " + chan + " :Cannot send to channel"
#define _ERR_CHANOPRIVSNEEDED(nick, chan) "482 " + nick + " " + chan + " :You're not channel operator"
// invite
#define _ERR_USERONCHANNEL(nick, chan) "443 " + nick + " Already on channel " + chan
#define _RPL_INVITING(nick, guest, chan) "341 " + nick + " " + guest + " " + chan
// mode
#define _ERR_USERSDONTMATCH(nick) "502 " + nick + " :Cant change mode for other users"
#define _ERR_UNKNOWNMODE(nick, modechar) "472 " + nick + " " + modechar + " :is unknown mode char to me"
#define _RPL_UMODEIS(nick, modes) "221 " + nick + " " + modes
#define _RPL_CHANNELMODEIS(nick, chan, modes, args) "324 " + nick + " " + chan + " " + modes + " " + args
// topic
#define _RPL_TOPICWHOTIME(nick, chan, setat) "333 " + nick + " " + chan + " " + nick + " " + setat
#define _RPL_TOPICCHANGESUCCESS(nick, chan, topic) nick + " changed the topic of " + chan + " to: " + topic
// list
#define _RPL_LISTSTART(nick) "321 " + nick + " Channel :Users Name"
#define _RPL_LIST(nick, chan, clientCount, topic) "322 " + nick + " " + chan + " " + clientCount + " :" + topic
#define _RPL_LISTEND(nick) "323 " + nick + " :End of LIST"
// kick
#define _ERR_USERNOTINCHANNEL(nick, user, chan) "441 " + nick + " " + nick + " " + chan + "  :They aren't on that channel"
#define _RPL_KICKSUCCESS(chan, nick, comment) "KICK " + chan + " " + nick + " " + comment
// kill
#define _ERR_NOPRIVILEGES(nick) "481 " + nick + " :Permission Denied- You're not an IRC operator"
#define _RPL_KILLSUCCESS(target) target + " has been killed"

class Server;

class Commands
{
	typedef std::map<std::string, void (Commands::*)(void)> cmdMap;

public:
	enum e_isPriv
	{
		_NOT_PRIV,
		_PRIV
	};
	enum e_isOper
	{
		_ISNOTOPER,
		_ISOPER
	};
	enum e_return
	{
		_OK,
		_ERROR
	};

	Commands(Server *server, User *user, std::string &str);
	~Commands();

private:
	void pass(void);
	void nick(void);
	void user(void);
	void whois(void);
	void who(void);
	void oper(void);
	void quit(void);
	void join(void);
	void finalizeJoin(User *user, Channel *channel);
	void part(void);
	void mode(void);
	void topic(void);
	void names(void);
	void name(Channel *channel, User *user);
	void list(void);
	void invite(void);
	void kick(void);
	void privmsg(bool isNoticeCmd);
	void kill(void);
	void ping(void);
	void pong(void);
	void squit(void);

	bool checkParamNbr(size_t nbr);
	void setupMap(void);
	void routeCmd(void);
	void registerClient(void);

	void broadcastToChannel(Channel *channel, std::string msg, bool isPriv);

	Server *_server;
	User *_user;
	std::vector<std::string> _params;
	cmdMap _cmdMap;
};

#endif
