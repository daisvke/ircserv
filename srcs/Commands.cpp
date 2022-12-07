/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:12 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/07 13:50:36 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "iterator"

Commands::Commands(Server *server, User *user, std::string &str)
	: _server(server), _user(user), _params(ircSplit(str, ' '))
{
	if (_params[0] == "CAP")
		return;

	std::string message = _ERR_PASSWDMISMATCH(_user->getNickName());
	_server->sendMsg(_user->getFd(), message);
	return ;
	std::cout << std::endl
			  << "/=================================" << std::endl;
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

std::string &Commands::getRpl() { return (_rpl); }

void Commands::setupMap() // define it and call once in Server ?
{
	_cmdMap["PASS"] = &Commands::pass;
	_cmdMap["NICK"] = &Commands::nick;
	_cmdMap["USER"] = &Commands::user;
	_cmdMap["WHOIS"] = &Commands::whois;
	//	_cmdMap["WHO"] = &Commands::who;
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
	_cmdMap["PING"] = &Commands::ping;
	_cmdMap["PONG"] = &Commands::pong;
}

// void	Commands::setupRplMap() // set up a map for answers
// {

// }

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
			std::string message = _ERR_NOSUCHCMD(cmd);
			return _server->sendMsg(_user->getFd(), message);
		}
	}
	if (_rpl.empty())
		_rpl = "test\r\n";
}

/*************************************************************
 * Sends channel message to one or more members of the channel
 *************************************************************/

void Commands::sendMsgToChan(Channel *channel, std::string &msg, bool isPriv)
{
	userDirectory 			*users = channel->getUserDirectory();
	userDirectory::iterator it = users->begin();
	std::string				privPrefix = isPriv ?  "PRIVMSG #" + channel->getName() + " :" : "";

	msg = privPrefix + msg;

	for (; it != users->end(); ++it)
		if ((*it).first != _user)
			_server->sendMsg((*it).first->getFd(), msg);
}

/*************************************************************
 * When launched with 'irssi -w <pwd>', a PASS message is sent to ircserv.
 * This function will check if the given pwd is correct.
 *************************************************************/
void Commands::pass(void)
{
	if (_params.size() < 2)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	if (_params[1] != _server->getPassword())
	{
		std::string message = _ERR_PASSWDMISMATCH(_user->getNickName());
		_server->sendMsg(_user->getFd(), message);
		_server->closeAllConn();
	}
}

/*************************************************************
 * Changes the user's nickname online
 * If no new nick is given, prints the current nick.
 *************************************************************/
void Commands::nick(void)
{
	std::string message;

	std::string newNick = _params[1];
	std::remove_if(newNick.begin(), newNick.end(), isspace);
	if (newNick.empty())
	{
		message = _ERR_NONICKNAMEGIVEN(newNick);
		return _server->sendMsg(_user->getFd(), message);
	}
	if (_params.size() == 1)
	{
		message = _RPL_CURRENTNICK(_user->getNickName());
		return _server->sendMsg(_user->getFd(), message);
	}

	if (_server->findUserByNick(newNick) == false)
	{
		_user->setNickName(newNick);
		message = _RPL_NICKSUCCESS(newNick);
		_server->sendMsg(_user->getFd(), message);
	}
	else
	{
		message = _ERR_NICKNAMEINUSE(newNick);
		_server->sendMsg(_user->getFd(), message);
	}
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
	if (_params.size() < 5)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
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

	_user->setUserName(_params[1]);
	_user->setHostName(_params[2]);
	_server->setName(_params[3]);
	std::string realName;
	if (_params[4][0] == ':')
	{
		_params[4].erase(0, 1);
	}
	for (size_t i(4); i < _params.size(); ++i)
		realName += ' ' + _params[i];
	_user->setRealName(realName);

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

	if (_params.size() < 2)
	{
		std::string message = _ERR_NONICKNAMEGIVEN(nick);
		return _server->sendMsg(_user->getFd(), message);
	}
	if (!_server->findUserByNick(nick))
	{
		message = _ERR_NOSUCHNICK(nick);
		return _server->sendMsg(_user->getFd(), message);
	}

	message = _RPL_WHOISREGNICK(nick);
	_server->sendMsg(_user->getFd(), message);
	message = _RPL_ENDOFWHOIS(nick);
	return _server->sendMsg(_user->getFd(), message);
}
/*
void Commands::who(void)
{
	std::string nick = _params[1], message;

	if (_params.size() < 2)
	{
		std::string message = _ERR_NONICKNAMEGIVEN(nick);
		return _server->sendMsg(_user->getFd(), message);
	}
	if (!_server->findUserByNick(nick))
	{
		message = _ERR_NOSUCHNICK(nick);
		return _server->sendMsg(_user->getFd(), message);
	}

	message = _RPL_WHOISREGNICK(nick);
	_server->sendMsg(_user->getFd(), message);
	message = _RPL_ENDOFWHOIS(nick);
	return _server->sendMsg(_user->getFd(), message);
}
*/

/*************************************************************
 * Used by a user to get oper privileges
 *************************************************************/
void Commands::oper(void)
{
	if (_params.size() < 3)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	User *user = _server->findUserByNick(_params[1]);
	if (!user)
		return;
	if (_server->getPassword() != _params[2])
	{
		std::string message = _ERR_PASSWDMISMATCH(_user->getNickName());
		return _server->sendMsg(_user->getFd(), message);
	}
	user->setAsOperator();
	std::string message = _RPL_YOUREOPER;
	_server->sendMsg(user->getFd(), message);
}

/*************************************************************
 * The user quits the network, which makes him leave all the channels as well
 *************************************************************/
void Commands::quit(void)
{
	std::string lastWords = _params[1].erase(0, 1);
	std::vector<Channel *> *channels = _server->getChannels();

	_user->disconnect();
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
					sendMsgToChan((*channels)[i], lastWords, _PRIV);
			}
		}
	}
}

/*************************************************************
 * The user begins listening to a channel
 *************************************************************/
void Commands::join(void)
{
	if (_params.size() < 2)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	std::vector<std::string> channelKeys;
	std::vector<std::string> channelNames = ircSplit(_params[1], ',');
	bool isOper;
	std::string message;

	if (_params.size() > 2)
		channelKeys = ircSplit(_params[2], ',');
	while (channelKeys.size() != channelNames.size())
		channelKeys.push_back("");

	for (size_t i(0); i < channelNames.size(); ++i)
	{
		std::string channelName = channelNames[i][0] == '#' ? channelNames[i].erase(0, 1) : channelNames[i];
		Channel *channel = _server->findChannel(channelName);

		if (!channel)
		{
			channel = _server->addChannel(channelName, channelKeys[i]);
			isOper = true;
		}
		else isOper = false;

		if (channel && channel->getUserDirectory()->count(_user))
			return;
		if (channel && (channel->isLimited() == false || (channel->isLimited() == true && channel->getUserNbr() < channel->getUserLimit())))
		{
			if (channel->isKeyProtected())
			{
				if (channelKeys[i] != channel->getKey())
				{
					message = _ERR_BADCHANNELKEY(channelName);
					return _server->sendMsg(_user->getFd(), message);
				}
			}
			if (channel->isInviteOnly())
			{
				message = _ERR_INVITEONLYCHAN(channelName);
				return _server->sendMsg(_user->getFd(), message);
			}
		}
		else if (channel && channel->isLimited() == true && channel->getUserNbr() >= channel->getUserLimit())
		{
			_ERR_CHANNELISFULL(channelName);
			return;
		}

		channel->join(_user, isOper);

		if (channel->getTopic().empty() == false)
		{
			message = _RPL_TOPIC(_user->getNickName(), channel->getName(), channel->getTopic());
			_server->sendMsg(_user->getFd(), message);
		}

		userDirectory *users = channel->getUserDirectory();
		for (userDirectory::iterator it(users->begin()); it != users->end(); ++it)
		{
			std::string nick = (*it).first->getNickName();
			std::string prefix = channel->isOper(nick) ? " :@" : " :";

			message = _RPL_NAMREPLY(_user->getNickName(), nick, '=', channel->getName(), prefix);
			_server->sendMsg(_user->getFd(), message);
		}
		message = _RPL_ENDOFNAMES(_user->getNickName(), channel->getName());
		_server->sendMsg(_user->getFd(), message);

		// If channel already existed, tell all users that a user has joined
		if (isOper == false)
		{
			std::string message = "JOIN #" + channelName;
			sendMsgToChan(channel, message, _NOT_PRIV);
		}
	}

	// handle? => user shouldn't be banned  ERR_BANNEDFROMCHAN
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
		message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	std::vector<std::string> channelNames = ircSplit(_params[1], ',');
	std::string reason;
	if (_params.size() > 2)
		reason = _params[2].erase(0, 1);

	for (size_t i(0); i < channelNames.size(); ++i)
	{
		std::string channelName = channelNames[i][0] == '#' ? channelNames[i].erase(0, 1) : channelNames[i];
		Channel *channel = _server->findChannel(channelName);

		if (!channel)
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
				users->erase((*it).first);
				// Tell all users that a user has left the channel
				message = "PART #" + channelName;
				_server->sendMsg(_user->getFd(), message);

				if (channel->isEmpty())
					_server->deleteChannel(channel->getName());
		//		message = _ERR_NOSUCHCHANNEL(_user->getNickName(), channelName);
		//		_server->sendMsg(_user->getFd(), message);
				break ;
			}
		}
		if (it == users->end())
		{
			std::string message = _ERR_NOTONCHANNEL(_user->getNickName(), channel->getName());
			return _server->sendMsg(_user->getFd(), message);
		}
	}
}

/*************************************************************
 * Used by a channel oper to change the mode of the channel.
 * The mode can be add ('+'), or removed ('-'). By default it is added.
 *************************************************************/
void Commands::mode(void)
{
	std::string message;

	if (_params.size() < 1)
	{
		message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	if (_params[1][0] == '#')
	{
		Channel *channel = _server->findChannel(_params[1].erase(0, 1));
		if (!channel)
		{
			message = _ERR_NOSUCHCHANNEL(_user->getNickName(), _params[1]);
			return _server->sendMsg(_user->getFd(), message);
		}
		if (_params.size() < 3)
		{
			message = *channel->getUserMode(_user->getNickName());
			return _server->sendMsg(_user->getFd(), message);
		}

		bool isChanOper = channel->isOper(_user->getNickName());

		if (isChanOper == false)
		{
			message = _ERR_CHANOPRIVSNEEDED(_user->getNickName());
			return _server->sendMsg(_user->getFd(), message);
		}

		bool remove = _params[2].find('-') ? true : false;
		char sign = remove == true ? '-' : '+';
		std::string modes = _params[2];
		modes.erase(std::remove(modes.begin(), modes.end(), sign), modes.end());

		std::string params = _params[3];
		for (size_t i(0); i < modes.size(); ++i)
			channel->modifyModes(modes[i], params, remove);
	}
}

/*************************************************************
 * If a topic name is given: changes the topic name of a channel,
 *	or adds one if no topic has been set
 * If no topic name is given, prints the current topic name
 *************************************************************/
void Commands::topic(void)
{
	std::string nickName = _user->getNickName(), message;

	if (_params.size() < 2)
	{
		message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	Channel *channel = _server->findChannel(_params[1].erase(0, 1));
	if (!channel)
	{
		message = _ERR_NOSUCHCHANNEL(nickName, _params[1]);
		return _server->sendMsg(_user->getFd(), message);
	}
	if (_params.size() == 2)
	{
		message = channel->getTopic();
		_server->sendMsg(_user->getFd(), message);
	}

	std::string newTopic = _params[2];
	if (channel->isTopicProtected() == false || _user->isOperator() == true)
		channel->setTopic(newTopic);
	else
	{
		message = _ERR_CHANOPRIVSNEEDED(nickName);
		return _server->sendMsg(_user->getFd(), message);
	}
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

		if (!(channels[i]->isTopicProtected() || channels[i]->isPrivate()))
		{
			std::cout << channels[i]->getName() << std::endl; // replace print fct
			channels[i]->names();
		}
	}
}

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
		std::vector<Channel *> channels = *_server->getChannels();

	for (size_t i(0); i < channels.size(); ++i)
	{

		if (!(channels[i]->isTopicProtected() || channels[i]->isPrivate()))
		{
			int userFd = _user->getFd();
			message = channels[i]->getName();
			_server->sendMsg(userFd, message);
			message = channels[i]->getTopic();
			_server->sendMsg(userFd, message);
		}
	}
}

void Commands::invite(void)
{
	std::string message;

	if (_params.size() < 4)
	{
		message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	std::string nick = _params[1];
	if (!_server->findUserByNick(nick))
	{
		message = _ERR_NOSUCHNICK(nick);
		return _server->sendMsg(_user->getFd(), message);
	}
	Channel *channel = _server->findChannel(_params[2]);

	if (channel->isInviteOnly() && channel->isOper(nick) == false)
	{
		message = _ERR_CHANOPRIVSNEEDED(_user->getNickName());
		return _server->sendMsg(_user->getFd(), message);
	}

	User *user = _server->findUserByNick(nick);
	userDirectory *users = channel->getUserDirectory();
	userDirectory::iterator it = users->begin();

	for (; it != users->end(); ++it)
		if ((*it).first->getNickName() == nick)
		{
			std::string message = _ERR_USERONCHANNEL(nick, _params[2]);
			return _server->sendMsg(_user->getFd(), message);
		}
	return channel->join(user, _ISNOTOPER);
}

/*************************************************************
 * Only a channel operator can kick out a user from the channel
 *************************************************************/
void Commands::kick(void)
{
	if (_params.size() < 3)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
		return _server->sendMsg(_user->getFd(), message);
	}

	User *target = _server->findUserByNick(_params[2]);
	std::string user = _user->getNickName();
	Channel *channel = _server->findChannel(_params[1]);

	if (!channel)
	{
		return; /* ERR_NOSUCHCHANNEL */
	}
	if (channel->isOper(user) == false)
	{
		std::string message = _ERR_CHANOPRIVSNEEDED(_user->getNickName());
		return _server->sendMsg(_user->getFd(), message);
	}

	channel->part(target);

	if (_params.size() > 3)
	{
		std::string comment = _params[3];
		std::cout << comment << std::endl; // replace print fct
	}
	// handle err_notonchannel ?
}

/*************************************************************
 * Unlike privmsg, notice does not send error messages
 *************************************************************/
void Commands::privmsg(bool isNoticeCmd)
{
	std::vector<std::string> names = ircSplit(_params[1], ',');
	std::string message = _params[2];
	std::string errMessage;
	std::string nickName = _user->getNickName();

	if (_params.size() < 3)
	{
		if (isNoticeCmd == false)
		{
			errMessage = _ERR_NEEDMOREPARAMS;
			return _server->sendMsg(_user->getFd(), errMessage);
		}
	}

	for (size_t i(0); i < names.size(); ++i)
	{
		if (names[i][0] == '#')
		{
			std::string	name = names[i].erase(0, 1);
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

			sendMsgToChan(channel, message, _PRIV);
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
			_server->sendMsg(target->getFd(), message);
		}
	}
}

void Commands::kill(void)
{
	if (_params.size() < 3)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
		_server->sendMsg(_user->getFd(), message);
		return;
	}
	if (_user->isOperator() == false)
	{
		return; /*ERR_NOPRIVILEGES*/
	}

	User *target = _server->findUserByNick(_params[1]);
	if (!target)
	{
		return; /* ERR_NOSUCHNICK */
	}

	std::string msg = "QUIT" + _params[1];
	Commands cmd(_server, target, msg);
}

void Commands::ping(void)
{
	std::string message, servername = _params[1];

	if (servername != _server->getName())
	{
		message = _ERR_NOSUCHSERVER(_server->getName());
		return _server->sendMsg(_user->getFd(), message);
	}
	message = "PING";
	_server->sendMsg(_user->getFd(), message);
}

void Commands::pong(void)
{
	std::string message;

	message = "PONG";
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

// Commands::Commands(Server *server, User *user, t_message msg)
// 	: _server(server), _user(user), _message(msg) { routeCmd(); }

// void	Commands::routeCmd()
// {
// 	switch (_message.cmd)
// 	{
// 		case _NICK:		nick(); break;
// 		case _USER:		user(); break;
// 		case _OPER:		oper(); break;
// 		case _QUIT:		quit(); break;
// 		case _JOIN:		join(); break;
// 		case _PART:		part(); break;
// 		case _MODE:		mode(); break;
// 		case _TOPIC:	topic(); break;
// 		case _NAMES:	names(); break;
// 		case _LIST:		list(); break;
// 		case _INVITE:	invite(); break;
// 		case _KICK:		kick(); break;
// 		case _KILL:		kill(); break;

// 		default:		std::cerr << "erroooor cmd not found" << std::endl; //replace fct
// 	}
// }
