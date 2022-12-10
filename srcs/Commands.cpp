/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:12 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/10 00:54:31 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "iterator"

Commands::Commands(Server *server, User *user, std::string &str)
	: _server(server), _user(user), _params(ircSplit(str, ' '))
{
	if (_params[0] == "CAP")
		return;

	std::cout << std::endl
			  << "/========received msg ===========" << std::endl;
	for (size_t i(0); i < _params.size(); ++i)
	{
		std::cout << _params[i] << std::endl;
	}
	std::cout << "=================================/" << std::endl
			  << std::endl;

	setupMap();
	routeCmd();
}

Commands::~Commands() {}

void Commands::setupMap() // define it and call once in Server ?
{
	_cmdMap["PASS"] = &Commands::pass;
	_cmdMap["NICK"] = &Commands::nick;
	_cmdMap["USER"] = &Commands::user;
	_cmdMap["WHOIS"] = &Commands::whois;
	_cmdMap["WHO"] = &Commands::who;
	_cmdMap["OPER"] = &Commands::oper;
	_cmdMap["QUIT"] = &Commands::quit;
	_cmdMap["JOIN"] = &Commands::join;
	_cmdMap["PART"] = &Commands::part;
	_cmdMap["MODE"] = &Commands::mode;
	_cmdMap["TOPIC"] = &Commands::topic;
	_cmdMap["NAMES"] = &Commands::names;
	_cmdMap["LIST"] = &Commands::list;
	_cmdMap["INVITE"] = &Commands::invite;
	_cmdMap["KICK"] = &Commands::kick;
	_cmdMap["KILL"] = &Commands::kill;
	_cmdMap["kill"] = &Commands::kill;
	_cmdMap["PING"] = &Commands::ping;
	_cmdMap["PONG"] = &Commands::pong;
}

/*************************************************************
 * Looks for the command on the cmdMap.
 * If nothing is found, sends an error message to the user.
 *************************************************************/
void Commands::routeCmd()
{
	cmdMap::iterator it;
	std::string cmd = _params[0];

	if (cmd == "PRIVMSG")
		privmsg(false);
	else if (cmd == "NOTICE")
		privmsg(true);
	else
	{
		it = _cmdMap.find(cmd);
		if (it != _cmdMap.end())
			(this->*_cmdMap[cmd])();
		else
		{
			std::string message = _ERR_UNKNOWNCOMMAND(_user->getNickName(), cmd);
			return _server->sendMsg(_user->getFd(), message);
		}
	}
}

/*************************************************************
 * Sends channel message to one or more members of the channel
 *************************************************************/
void	Commands::broadcastToChannel(Channel *channel, std::string msg, bool isPriv)
{
	userDirectory 			*users = channel->getUserDirectory();


	for (userDirectory::iterator it = users->begin(); it != users->end(); ++it)
	{
		if ((*it).first != _user)
		{
			msg = isPriv ? "PRIVMSG " + channel->getName() + " " + msg : msg;
			int			userFd = (*it).first->getFd();
			std::string userId  = _user->getId();
			_server->sendMessage(userFd, userId, msg);
		}
	}
}

/*************************************************************
 * When launched with 'irssi -w <pwd>', a PASS message is sent to ircserv.
 * This function will check if the given pwd is correct.
 *************************************************************/
void Commands::pass(void)
{
	int	userFd = _user->getFd();
	if (_params.size() < 2)
	{
		std::string message = _ERR_NEEDMOREPARAMS(_user->getNickName(), _params[0]);
		return _server->sendMsg(userFd, message);
	}

	if (_params[1] != _server->getPassword())
	{
		std::string message = _ERR_PASSWDMISMATCH(_user->getNickName());
		_server->sendMsg(userFd, message);
		_server->closeFd(userFd);
	}
	else
		_user->setIsPwdVerified();
}

/*************************************************************
 * Changes the user's nickname online
 * If no new nick is given, prints the current nick.
 *************************************************************/
void Commands::nick(void)
{
	std::string message;

	if (_user->isPwdVerified() == false)
	{
		std::string message = _ERR_PASSWDMISMATCH(_user->getNickName());
		_server->sendMsg(_user->getFd(), message);
		_server->closeFd(_user->getFd());
	}

	std::string newNick = _params[1];
	std::remove_if(newNick.begin(), newNick.end(), isspace);

	if (newNick.empty())
	{
		message = _ERR_NONICKNAMEGIVEN;
		return _server->sendMsg(_user->getFd(), message);
	}
	if (newNick.size() > 9 || !isalpha(*(newNick.begin())))
	{
		message = _ERR_ERRONEUSNICKNAME(newNick);
		return _server->sendMsg(_user->getFd(), message);
	}
	if (_params.size() == 1)
	{
		message = _RPL_CURRENTNICK(_user->getNickName());
		return _server->sendMsg(_user->getFd(), message);
	}
	if (_server->findUserByNick(newNick) != 0)
	{
		message = _ERR_NICKNAMEINUSE(newNick);
		_server->sendMsg(_user->getFd(), message);
		return _server->closeFd(_user->getFd());
	}
	_user->setNickName(newNick);
	message = _RPL_NICKSUCCESS(newNick);
	_server->sendMsg(_user->getFd(), message);
}

/*************************************************************
 * Used at the begining of a connection between client and server.
 * The string sent by the client contains:
 * 	<username> <hostname> <servername> <realname>
 * This cmd will assign all this data to the User object.
 * It removes the ':' character if it set before realname.
 *************************************************************/
void Commands::user(void)
{
	std::string message;
	if (_params.size() < 5)
	{
		message = _ERR_NEEDMOREPARAMS(_user->getNickName(), _params[0]);
		return _server->sendMsg(_user->getFd(), message);
	}

//	std::string userName = _params[1];
	/*	if (_server->findUserByName(userName) == false)
			_user->setUserName(userName);
		else
		{
			std::string	message = _ERR_ALREADYREGISTRED(userName);
			return _server->sendMsg(_user->getFd(), message);
		}
	*/
	//_user->setUserName(userName);

	if (_user->getNickName().empty() == true)
	{
		_server->closeFd(_user->getFd());
		message = _ERR_NONICKNAMEGIVEN;
		return _server->sendMsg(_user->getFd(), message);
	}

	_user->setUserName(_params[1]);
	_user->setHostName(_params[2]);
	_server->setName(_params[3]);
	std::string realName;
	if (_params[4][0] == ':')
		_params[4].erase(0, 1);
	for (size_t i(4); i < _params.size(); ++i)
		realName += ' ' + _params[i];
	_user->setRealName(realName);

	std::string	newId = _user->getNickName() + "!" + _user->getUserName() + "@" + _server->getName();
	_user->setId(newId);

	registerClient();
}

void Commands::registerClient(void)
{
	std::string nickName = _user->getNickName();
	std::string serverName = _server->getName();

	std::string message = _RPL_WELCOME(nickName, _user->getUserName());
	_server->sendMsg(_user->getFd(), message);
	message = _RPL_YOURHOST(nickName, serverName);
	_server->sendMsg(_user->getFd(), message);
	message = _RPL_CREATED(nickName, getTimeStr());
	_server->sendMsg(_user->getFd(), message);
	message = _RPL_MYINFO(nickName, serverName);
	_server->sendMsg(_user->getFd(), message);
}

void Commands::whois(void)
{
	std::string nick = _params[1], message;
	User		*user;

	if (_params.size() < 2)
	{
		message = _ERR_NONICKNAMEGIVEN;
		return _server->sendMsg(_user->getFd(), message);
	}
	if (!(user = _server->findUserByNick(nick)))
	{
		message = _ERR_NOSUCHNICK(nick);
		return _server->sendMsg(_user->getFd(), message);
	}

	message = _RPL_WHOISREGNICK(nick);
	_server->sendMsg(_user->getFd(), message);
	if (user->isOperator())
	{
		message = _RPL_WHOISOPERATOR(nick);
		_server->sendMsg(_user->getFd(), message);
	}
	message = _RPL_ENDOFWHOIS(nick);
	return _server->sendMsg(_user->getFd(), message);
}

void Commands::who(void)
{
	if (_params.size() < 2) return ;
	std::string nick = _params[1], message;

	if (!_server->findUserByNick(nick)) return ;

	message = _RPL_WHOISREGNICK(nick);
	_server->sendMsg(_user->getFd(), message);
	message = _RPL_ENDOFWHOIS(nick);
	return _server->sendMsg(_user->getFd(), message);
}

/*************************************************************
 * Used by a user to get oper privileges
 *************************************************************/
void Commands::oper(void)
{
	std::string	userNick = _user->getNickName();

	if (_params.size() < 3)
	{
		std::string message = _ERR_NEEDMOREPARAMS(userNick, _params[0]);
		return _server->sendMsg(_user->getFd(), message);
	}

	User *user = _server->findUserByNick(_params[1]);
	if (!user)
		return;
	if (_server->getPassword() != _params[2])
	{
		std::string message = _ERR_PASSWDMISMATCH(userNick);
		return _server->sendMsg(_user->getFd(), message);
	}
	user->setAsOperator();
	std::string message = _RPL_YOUREOPER(userNick);
	_server->sendMsg(user->getFd(), message);
}

/*************************************************************
 * The user quits the network, which makes him leave all the channels as well
 *************************************************************/
void Commands::quit(void)
{
	std::string lastWords = _params[1].erase(0, 1);
	std::vector<Channel *> *channels = _server->getChannels();

	for (size_t i(0); i < channels->size(); ++i)
	{
		userDirectory *users = (*channels)[i]->getUserDirectory();
		userDirectory::iterator it = users->begin();

		for (; it != users->end(); ++it)
		{
			if ((*it).first->getNickName() == _user->getNickName())
			{
				(*channels)[i]->part((*it).first);
				if (lastWords.empty() == false)
					broadcastToChannel((*channels)[i], lastWords, _PRIV);
				return ;
			}
		}
	}
}

/*************************************************************
 * The user begins listening to a channel
 *************************************************************/
void Commands::join(void)
{
	std::string message;
	std::string	userNick = _user->getNickName();
	int			userFd = _user->getFd();

	if (_params.size() < 2)
	{
		message = _ERR_NEEDMOREPARAMS(userNick, _params[0]);
		return _server->sendMsg(userFd, message);
	}
	std::vector<std::string> channelKeys;
	std::vector<std::string> channelNames = ircSplit(_params[1], ',');
	bool isOper;

	if (_params.size() > 2)
		channelKeys = ircSplit(_params[2], ',');
	/*just added, add # before channelName + troncate if too long*/
	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		if ((channelNames[i].at(0)) != '#')
			channelNames[i] = "#" + channelNames[i];
		if (channelNames[i].size() > 50)
			channelNames[i].erase(channelNames[i].begin() + 50, channelNames[i].end());
		// std::string::iterator it = channelNames[i].begin();
		// for (; it < channelNames[i].end(); ++it)
    	// 	*it = std::tolower(*it);
	}
	/*just added, delete if not useful*/

	while (channelKeys.size() != channelNames.size())
		channelKeys.push_back("");

	for (size_t i(0); i < channelNames.size(); ++i)
	{
		std::string channelName = channelNames[i];
		Channel *channel;

		if (!(channel = _server->findChannel(channelName)))
		{
			channel = _server->addChannel(channelName, channelKeys[i]);
			isOper = true;
		}
		else isOper = false;

		if (channel->getUserDirectory()->count(_user))
			return;
		if (channel->isLimited() == false
			|| (channel->isLimited() == true && channel->countUsers() < channel->getUserLimit()))
		{
			if (channel->isKeyProtected())
			{
				if (channelKeys[i] != channel->getKey())
				{
					message = _ERR_BADCHANNELKEY(userNick, channelName);
					return _server->sendMsg(userFd, message);
				}
			}
			if (channel->isInviteOnly())
			{
				std::cout << "\033[32m =====================> invite 1\033[0m" << std::endl;

				message = _ERR_INVITEONLYCHAN(userNick, channelName);
				return _server->sendMsg(userFd, message);
			}
		}
		else if (channel->isLimited() == true && channel->countUsers() >= channel->getUserLimit())
		{
					std::cout << "\033[33m =====================> limited\033[0m" << std::endl;

			_ERR_CHANNELISFULL(userNick, channelName);
			return _server->sendMsg(userFd, message);
		}

		channel->join(_user, isOper);

		message = "JOIN " + channelName;
		broadcastToChannel(channel, message, _NOT_PRIV);
		_server->sendMessage(userFd, _user->getId(), message);


		// Print channel parameters to user
		if (channel->getTopic().empty() == false)
		{
			message = _RPL_TOPIC(userNick, channel->getName(), channel->getTopic());
			_server->sendMsg(userFd, message);
		}

		userDirectory *users = channel->getUserDirectory();
		for (userDirectory::iterator it(users->begin()); it != users->end(); ++it)
		{
			std::string nick = (*it).first->getNickName();
			std::string prefix = channel->isOper(nick) ? ":@" : ":";

			message = _RPL_NAMREPLY(userNick, nick, '=', channel->getName(), prefix);
			_server->sendMsg(userFd, message);
		}
		message = _RPL_ENDOFNAMES(userNick, channel->getName());
		_server->sendMsg(userFd, message);
	}
}

/*************************************************************
 * The user leaves all the channels given as parameters.
 * If no parameter is given, the current channel is left.
 *************************************************************/
void Commands::part(void)
{
	std::string	message;
	if (_params.size() < 2)
	{
		message = _ERR_NEEDMOREPARAMS(_user->getNickName(), _params[0]);
		return _server->sendMsg(_user->getFd(), message);
	}

	std::vector<std::string> channelNames = ircSplit(_params[1], ',');
	std::string reason;
	if (_params.size() > 2)
		reason = _params[2].erase(0, 1);

	for (size_t i(0); i < channelNames.size(); ++i)
	{
		std::string channelName = channelNames[i];
		Channel		*channel;

		if (!(channel = _server->findChannel(channelName)))
		{
			message = _ERR_NOSUCHCHANNEL(_user->getNickName(), channelName);
			_server->sendMsg(_user->getFd(), message);
			continue;
		}

		userDirectory *users = channel->getUserDirectory();
		userDirectory::iterator it = users->begin();
		while (it != users->end())
		{
			if ((*it).first->getNickName() == _user->getNickName())
			{
				// Tell all users that a user has left the channel
				message = "PART " + channelName;
				broadcastToChannel(channel, message, _NOT_PRIV);
				_server->sendMessage(_user->getFd(), _user->getId(), message);

				users->erase((*it).first);

				if (channel->isEmpty())
					_server->deleteChannel(channel->getName());
				break ;
			}
			++it;
		}
		if (it == users->end())
		{
			std::string message = _ERR_NOTONCHANNEL(_user->getNickName(), channel->getName());
			return _server->sendMsg(_user->getFd(), message);
		}
	}
}

/*************************************************************
 * MODE <target> [<modestring> [<mode arguments>...]]
 * Used by a channel oper to change the mode of the channel.
 * The mode can be add ('+'), or removed ('-').
 * Parsing concerning the signs are done by the reference client Irssi
 *************************************************************/
void Commands::mode(void)
{
	std::string 			concatParams, message, mode, modes;
	std::string				userNick = _user->getNickName();
	int						userFd = _user->getFd();
	char					sign;
	std::map<char, char>	foundModes;

	if (_params.size() < 2)
	{
		message = _ERR_NEEDMOREPARAMS(userNick, _params[0]);
		return _server->sendMsg(userFd, message);
	}
	if (_params[1][0] == '#') // channel modes
	{
		Channel *channel = _server->findChannel(_params[1]);
		if (!channel)
		{
			message = _ERR_NOSUCHCHANNEL(userNick, _params[1]);
			return _server->sendMsg(userFd, message);
		}
		// If no modestring is given, only prints the current channel modes
		if (_params.size() < 3)
		{
			int	userLimit = channel->getUserLimit();
			concatParams = userLimit > 0 ? toString(userLimit) : "";
			message = _RPL_CHANNELMODEIS(userNick, channel->getName(), \
										channel->getModes(), concatParams);
			return _server->sendMsg(userFd, message);
		}

		modes = _params[2];
		std::vector<std::string> params;

		// Get mode parameters if found
		if (_params.size() > 3)
			for (size_t i(3); i < _params.size(); ++i)
				params.push_back(_params[i]);

		if (_params.size() > 2) // modestring present: parse
		{
			size_t	i = 0;
			while (i < modes.size())
			{
				if (modes[i] == '-' || modes[i] == '+')
				{
					while (i < modes.size() && (modes[i] == '-' || modes[i] == '+')) {
						sign = modes[i];
						++i;
					}
				}
				else return ;

				std::string channelModes = _CHANMODES;
				std::string	paramModes = _CHAN_PARAM_MODES;
				bool		needsParam;

				while (i < modes.size() && !(modes[i] == '-' || modes[i] == '+'))
				{
					// If the found mode needs a param that is not found, ignore it
					if (paramModes.find(modes[i]) != std::string::npos)
					{
						needsParam = true;
						if ((params.empty() == true && sign != '-') || (!params.empty() && sign == '-'))
						{
							++i;
							continue ;
						}
					}
					else if (channelModes.find(modes[i]) == std::string::npos)
					{
						message = _ERR_UNKNOWNMODE(userNick, modes[i]);
						_server->sendMsg(userFd, message);
						++i;
						continue;
					}
					// Save validated mode with its sign
					if ((channel->getModes().find(modes[i]) == std::string::npos && sign == '+')
					|| (channel->getModes().find(modes[i]) != std::string::npos && sign == '-')
					|| (channel->getModes().find(modes[i]) != std::string::npos && needsParam == true))
						foundModes[modes[i]] = sign;
					++i;
				}
			}
		}
		if (foundModes.empty() == false)
		{
			bool isChanOper = channel->isOper(userNick);
			if (isChanOper == false)
			{
				message = _ERR_CHANOPRIVSNEEDED(userNick, channel->getName());
				return _server->sendMsg(userFd, message);
			}
			// Apply mode modifications
			std::map<char, char>::iterator	it;
			size_t							i = 0;
			std::string						concatModes;
			bool							changed;
			bool							changedAtLeastOnce = false;
			char							oldSign;

			for (it = foundModes.begin(); it != foundModes.end(); ++it)
			{
				std::string	tmpParams = i < params.size() ? params[i++] : "";
				changed = channel->modifyModes((*it).first, tmpParams, (*it).second);
				if (changed == true) {
					changedAtLeastOnce = true;
					if (it == foundModes.begin() || (*it).second != oldSign)
						concatModes += toString((*it).second) + (*it).first;
					else
						concatModes += (*it).first;
					oldSign = (*it).second;
					concatParams += tmpParams;
				}
			}
			if (changedAtLeastOnce)
			{
				// Broadcast mode update to all members
				message = _RPL_CHANNELMODEIS(userNick, channel->getName(), \
											concatModes, concatParams);
				broadcastToChannel(channel, message, _NOT_PRIV);
				return _server->sendMsg(userFd, message);
			}
		}

	}
	else // user modes
	{
		std::string targetNick = _params[1];
		User		*user = _server->findUserByNick(userNick);
		if (!user)
		{
			message = _ERR_NOSUCHNICK(userNick);
			return _server->sendMsg(_user->getFd(), message);
		}
		if (targetNick != userNick)
		{
			message = _ERR_USERSDONTMATCH(userNick);
			return _server->sendMsg(_user->getFd(), message);
		}
		// If no modestring is given, print current modes
		if (_params.size() < 3)
		{
			mode = _user->isOperator() ? "o" : "";
			message = _RPL_UMODEIS(userNick, mode);
			return _server->sendMsg(userFd, message);
		}
	}
}

/*************************************************************
 * If a topic name is given: changes the topic name of a channel,
 *	or adds one if no topic has been set
 * If no topic name is given, prints the current topic name
 *************************************************************/
void Commands::topic(void)
{
	std::string userNick = _user->getNickName(), message;
	int			userFd = _user->getFd();

	if (_params.size() < 2)
	{
		message = _ERR_NEEDMOREPARAMS(userNick, _params[0]);
		return _server->sendMsg(userFd, message);
	}

	std::string	fullTopicName = concatArrayStrs(_params, 2);
	std::string	channelName = _params[1];

	Channel *channel = _server->findChannel(channelName);
	if (!channel)
	{
		message = _ERR_NOSUCHCHANNEL(userNick, channelName);
		return _server->sendMsg(userFd, message);
	}
	if (_params.size() == 2)
	{
		message = _RPL_TOPIC(userNick, channelName, fullTopicName);
		return _server->sendMsg(userFd, message);
	}
	if (channel->isTopicProtected() == false || _user->isOperator() == true)
	{
		channel->setTopic(fullTopicName);
		message = "TOPIC " + channelName + " " + fullTopicName;
		broadcastToChannel(channel, message, _NOT_PRIV);
		_server->sendMessage(userFd, _user->getId(), message);
	}
	message = _ERR_CHANOPRIVSNEEDED(userNick, channelName);
	_server->sendMsg(userFd, message);
}

/*************************************************************
 * This command is built-in on our reference client Irssi
 *
 * If no channel name is given as param:
 *	all the channels with all its members are printed.
 * If channel names are given:
 * 	Prints all members of all the given non private, non secret, or currently listening channels.
 * There is no error message for wrong channel names etc.
 *************************************************************/
void Commands::names(void)
{
	std::vector<Channel *> channels;

	if (_params.size() > 1)
	{
		std::vector<std::string> channelNames = ircSplit(_params[1], ',');
		for (size_t i(0); i < channelNames.size(); ++i)
			if (Channel *chan = _server->findChannel(channelNames[i]))
				channels.push_back(chan);
	}
	else
		channels = *_server->getChannels();

	for (size_t i(0); i < channels.size(); ++i)
	{

		if (channels[i]->isTopicProtected() == false)
		{
			std::cout << channels[i]->getName() << std::endl; // replace print fct
			channels[i]->names();
		}
	}
}

/*************************************************************
* If no parameter is given, prints info about all the channels.
* If channel names are given, they are the only onces to be handled.
 *************************************************************/
void Commands::list(void)
{
	std::vector<Channel *> channels;
	std::string message;

	if (_params.size() > 1)
	{
		std::vector<std::string> channelNames = ircSplit(_params[1], ',');
		for (size_t i(0); i < channelNames.size(); ++i)
			if (Channel *chan = _server->findChannel(channelNames[i]))
				channels.push_back(chan);
	}
	else
		channels = *_server->getChannels();

	int 		userFd = _user->getFd();
	std::string	userNick = _user->getNickName();

	message = _RPL_LISTSTART(userNick);
	_server->sendMsg(userFd, message);

	for (size_t i(0); i < channels.size(); ++i)
	{
		if (channels[i]->isTopicProtected() == false)
		{
			std::string	userCount = toString(channels[i]->countUsers());
			message = _RPL_LIST(userNick, channels[i]->getName(), userCount, channels[i]->getTopic());
			_server->sendMsg(userFd, message);
		}
	}
	message = _RPL_LISTEND(userNick);
	_server->sendMsg(userFd, message);
}

void Commands::invite(void)
{
	std::string	userNick = _user->getNickName();
	int			userFd = _user->getFd();
	std::string message;

	if (_params.size() < 3)
	{
		message = _ERR_NEEDMOREPARAMS(userNick, _params[0]);
		return _server->sendMsg(userFd, message);
	}

	std::string guestNick = _params[1];
	User	*guest =  _server->findUserByNick(userNick);
	int		guestFd = guest->getFd();
	if (!guest)
	{
		message = _ERR_NOSUCHNICK(userNick);
		return _server->sendMsg(userFd, message);
	}

	std::string	chanName = _params[2];
	Channel *channel = _server->findChannel(chanName);
	if (!channel)
	{
		message = _ERR_NOSUCHCHANNEL(userNick, _params[2]);
		return _server->sendMsg(userFd, message);
	}

	if (channel->isInviteOnly() && channel->isOper(userNick) == false)
	{
		message = _ERR_CHANOPRIVSNEEDED(userNick, channel->getName());
		return _server->sendMsg(userFd, message);
	}

	userDirectory *users = channel->getUserDirectory();
	userDirectory::iterator it = users->begin();

	for (; it != users->end(); ++it)
	{
		if ((*it).first->getNickName() == guestNick)
		{
			std::string message = _ERR_USERONCHANNEL(guestNick, chanName);
			return _server->sendMsg(userFd, message);
		}
	}
	message = _RPL_INVITING(userNick, guestNick, _params[2]);
	_server->sendMsg(userFd, message);
	_server->sendMsg(guestFd, message);

	return channel->join(guest, _ISNOTOPER);
}

/*************************************************************
 * Only a channel operator can kick out a user from the channel
 *************************************************************/
void Commands::kick(void)
{
	std::string	userNick = _user->getNickName();
	int			userFd = _user->getFd();
	std::string message, targetNick = _params[2];

	if (_params.size() < 3)
	{
		message = _ERR_NEEDMOREPARAMS(userNick, _params[0]);
		return _server->sendMsg(userFd, message);
	}

	Channel *channel;
	if (!(channel = _server->findChannel(_params[1])))
	{
		message = _ERR_NOSUCHCHANNEL(userNick, _params[1]);
		return _server->sendMsg(userFd, message);
	}

	std::string	channelName = channel->getName();
	if (channel->isMember(targetNick) == false)
	{
		message = _ERR_USERNOTINCHANNEL(userNick, targetNick, channelName);
		return _server->sendMsg(userFd, message);
	}

	User *target = _server->findUserByNick(targetNick);
	if (!target)
	{
		message = _ERR_NOSUCHNICK(targetNick);
		return _server->sendMsg(_user->getFd(), message);
	}

 	// Can only use KICK if oper
	if (channel->isOper(userNick) == false)
	{
		std::string message = _ERR_CHANOPRIVSNEEDED(userNick, channelName);
		return _server->sendMsg(userFd, message);
	}
	// Use the comment if it is given
	std::string comment;
	if (_params.size() > 3)
		comment = concatArrayStrs(_params, 3);

	// Kick target out of the channel using PART message
	message = "PART " + channelName;
	broadcastToChannel(channel, message, _NOT_PRIV);
	_server->sendMessage(target->getFd(), target->getId(), message);
	// Broadcast success message to all users on channel
	message = _RPL_KICKSUCCESS(channelName, target->getNickName(), comment);
	broadcastToChannel(channel, message, _NOT_PRIV);
	_server->sendMessage(_user->getFd(), _user->getId(), message);

	// kick target out on the server
	channel->part(target);
}

/*************************************************************
 * Unlike privmsg, notice does not send error messages
 *************************************************************/
void Commands::privmsg(bool isNoticeCmd)
{
	std::vector<std::string> names = ircSplit(_params[1], ',');
	std::string errMessage;
	std::string nickName = _user->getNickName();

	if (_params.size() < 3)
	{
		if (isNoticeCmd == false)
		{
			errMessage = _ERR_NEEDMOREPARAMS(_user->getNickName(), _params[0]);
			return _server->sendMsg(_user->getFd(), errMessage);
		}
	}

	std::string message = concatArrayStrs(_params, 2);

	for (size_t i(0); i < names.size(); ++i)
	{
		if (names[i][0] == '#')
		{
			std::string	name = names[i];
			Channel *channel = _server->findChannel(name);
			if (!channel)
			{
				errMessage = _ERR_NOSUCHCHANNEL(nickName, name);
				return _server->sendMsg(_user->getFd(), errMessage);
			}
			if (channel->isModerated() && !(_user->isOperator() || channel->hasVoice(nickName)))
			{
				errMessage = _ERR_CANNOTSENDTOCHAN(nickName, name);
				return _server->sendMsg(_user->getFd(), errMessage);
			}

			broadcastToChannel(channel, message, _PRIV);
		}
		else
		{
			User *target = _server->findUserByNick(names[i]);
			if (!target)
			{
				if (isNoticeCmd == false)
				{
					errMessage = _ERR_NOSUCHNICK(names[i]);
					return _server->sendMsg(_user->getFd(), errMessage);
				}
			}
			else
				_server->sendMsg(target->getFd(), message);
		}
	}
}

void Commands::kill(void)
{
	std::string	userNick = _user->getNickName();
	std::string message;
	int			userFd = _user->getFd();

	if (_params.size() < 3)
	{
		message = _ERR_NEEDMOREPARAMS(userNick, _params[0]);
		return _server->sendMsg(userFd, message);
	}
	if (_user->isOperator() == false)
	{
		message = _ERR_NOPRIVILEGES(userNick);
		return _server->sendMsg(userFd, message);
	}

	User 		*target = _server->findUserByNick(_params[1]);
	if (!target)
	{
		message = _ERR_NOSUCHNICK(userNick);
		return _server->sendMsg(userFd, message);
	}

	std::string comment = concatArrayStrs(_params, 2);
	std::string	targetNick = target->getNickName();

	// Killing target
	message = "KILL " + targetNick + " " + comment;
	int	targetFd = target->getFd();
	_server->sendMessage(targetFd, _user->getId(), message);
	_server->closeFd(targetFd);
	// KILL success notice
	message = _RPL_KILLSUCCESS(targetNick);
	_server->sendMessage(userFd, _user->getId(), message);
}

void Commands::ping(void)
{
	std::string message = "PING", servername = _params[1];

	if (servername != _server->getName())
	{
		message = _ERR_NOSUCHSERVER(_user->getNickName(), _server->getName());
		return _server->sendMsg(_user->getFd(), message);
	}
	_server->sendMsg(_user->getFd(), message);
}

void Commands::pong(void)
{
	std::string message = "PONG";
	_server->sendMsg(_user->getFd(), message);
}

/*************/
/* to delete */
/*************/
template <typename T>
void printMap(std::map<std::string, T> &mymap)
{
	for (typename std::map<std::string, T>::iterator it = mymap.begin(); it != mymap.end(); ++it)
	{
		std::cout << "key : " << it->first << " - "
				  << "val : " << it->second << std::endl;
	}
}
