/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:12 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/01 22:53:19 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "iterator"

Commands::Commands(Server *server, User *user, std::string &str)
	: _server(server), _user(user), _params(ircSplit(str, ' '))
{
	if (_params[0] == "CAP")
		return;
	setupMap();
	routeCmd();
}

Commands::~Commands() {}

std::string &Commands::getRpl() { return (_rpl); }

void Commands::setupMap()
{
	_cmdMap[NICK] = &Commands::nick;
	_cmdMap[USER] = &Commands::user;
	_cmdMap[OPER] = &Commands::oper;
	_cmdMap[QUIT] = &Commands::quit;
	_cmdMap[JOIN] = &Commands::join;
	_cmdMap[PART] = &Commands::part;
	_cmdMap[MODE] = &Commands::mode;
	_cmdMap[TOPIC] = &Commands::topic;
	_cmdMap[NAMES] = &Commands::names;
	_cmdMap[LIST] = &Commands::list;
	_cmdMap[INVITE] = &Commands::invite;
	_cmdMap[KICK] = &Commands::kick;
	_cmdMap[PRIVMSG] = &Commands::privmsg;
	_cmdMap[KILL] = &Commands::kill;
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

	it = _cmdMap.find(cmd);
	if (it != _cmdMap.end())
		(this->*_cmdMap[cmd])();
	else
	{
		std::string message = _ERR_NOSUCHCMD(cmd);
		_server->sendMsg(_user->getFd(), message);
		return;
	}

	if (_rpl.empty())
		_rpl = "test\r\n";
}

/*************************************************************
 * Sends channel message to one or more members of the channel
 *************************************************************/

void Commands::sendMsgToChan(Channel *channel, std::string &msg)
{
	userDirectory users = channel->getUserDirectory();
	userDirectory::iterator it = users.begin();

	for (; it != users.end(); ++it)
		_server->sendMsg((*it).first->getFd(), msg);
}

/*************************************************************
 * Changes the user's nickname online; for example /nick Carlos.
 * If no new nick is given, prints the current nick.
 *************************************************************/
void Commands::nick(void)
{
	std::string message;
	std::string newNick = _params[1];
	std::remove_if(newNick.begin(), newNick.end(), isspace);
	if (newNick.empty())
	{
		message = _ERR_NONICKNAMEGIVEN;
		_server->sendMsg(_user->getFd(), message);
		return;
	}
	if (_params.size() == 1)
	{
		message = _RPL_CURRENTNICK(_user->getNickName());
		_server->sendMsg(_user->getFd(), message);
		return;
	}

	if (_server->findUserByNick(newNick) == false)
	{
		_user->setNickName(newNick);
		message = _RPL_NICKSUCCESS(newNick);
		_server->sendMsg(_user->getFd(), message);
	}
	else
	{
		message = _ERR_NICKNAMEINUSE(newNick); //
		_server->sendMsg(_user->getFd(), message);
	}
}

/*************************************************************
 * Used at the begining of a connection
 *************************************************************/
void Commands::user(void)
{
	/*
	if (_params.size() < 2) {
		std::string	message = _ERR_NEEDMOREPARAMS;
		_server->sendMsg(_user->getFd(), message); return ;
	}
	*/

	std::string newUserName = _params[1];
	if (_server->findUserByName(newUserName) == false)
		_user->setUserName(newUserName);
	else
	{ /* ERR_ALREADYREGISTRED */
	}
}

/*************************************************************
 * Used by a user to get oper privileges
 *************************************************************/
void Commands::oper(void)
{
	if (_params.size() < 3)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
		_server->sendMsg(_user->getFd(), message);
		return;
	}

	User *user = _server->findUserByNick(_params[1]);
	if (!user)
	{
		return;
	}
	if (_server->getPassword() != _params[1])
	{
		std::string message = _ERR_PASSWDMISMATCH;
		_server->sendMsg(_user->getFd(), message);
		return;
	}
	_user->setAsOperator();
	std::string message = _RPL_YOUREOPER;
	_server->sendMsg(_user->getFd(), message);
}

/*************************************************************
 * The user quits the network, which makes him leave all the channels as well
 *************************************************************/
void Commands::quit(void)
{
	std::string lastWords = _params[1];
	std::vector<Channel *> channels = _server->getChannels();

	_user->disconnect();
	for (size_t i(0); i < channels.size(); ++i)
	{
		userDirectory users = channels[i]->getUserDirectory();
		userDirectory::iterator it = users.begin();

		for (; it != users.end(); ++it)
			if ((*it).first->getNickName() == _user->getNickName())
				channels[i]->part((*it).first);
	}
	if (lastWords.empty() == false)
		std::cout << lastWords << std::endl; // replace by server broadcast function
}

/*************************************************************
 * The user begins listening to a channel
 *************************************************************/
void Commands::join(void)
{
	if (_params.size() < 2)
	{
		std::string message = _ERR_NEEDMOREPARAMS;
		_server->sendMsg(_user->getFd(), message);
		return;
	}

	std::vector<std::string> channelNames = ircSplit(_params[1], ',');
	std::vector<std::string> channelKeys = ircSplit(_params[2], ',');

	for (size_t i(0); i < channelNames.size(); ++i)
	{
		Channel *channel = _server->findChannel(channelNames[i]);

		if (!channel && channel->getUserNbr() < channel->getUserLimit())
		{
			// create chan
			continue;
		}
		else if (!channel)
			_ERR_CHANNELISFULL(channelNames[i]);
		if (channel->isKeyProtected())
		{
			if (channelKeys[i] != channel->getKey())
			{
				std::string message = _ERR_BADCHANNELKEY(channelNames[i]);
				_server->sendMsg(_user->getFd(), message); return;
			}
		}
		if (channel->isInviteOnly())
		{
			std::string message = _ERR_INVITEONLYCHAN(channelNames[i]);
			_server->sendMsg(_user->getFd(), message); return ;
		}
		channel->join(_user);
		// RPL_TOPIC
	}

	// handle? => user shouldn't be banned  ERR_BANNEDFROMCHAN
	// when join succeeds, screen is cleared and
	// 	chan sends to user the topic name + list of
	// 	all users on the chan, with himself on the list as well
}

/*************************************************************
 *  The user quits all the channels given as parameters
 *************************************************************/
void Commands::part(void)
{
	if (_params.size() < 2)
	{
		return; /*ERR_NEEDMOREPARAMS;*/
	}

	std::vector<std::string> channelNames = ircSplit(_params[1], ',');

	for (size_t i(0); i < channelNames.size(); ++i)
	{
		Channel *channel = _server->findChannel(channelNames[i]);

		if (!channel)
		{
			continue; /* ERR_NOSUCHCHANNEL */
		}
		userDirectory users = channel->getUserDirectory();
		userDirectory::iterator it = users.begin();

		for (; it != users.end(); ++it)
			if ((*it).first->getNickName() == _user->getNickName())
				channel->part((*it).first);
		channel->part(_user);
	}
}

/*************************************************************
 * Used by a channel oper to change the mode of the channel.
 * The mode can be add ('+'), or removed ('-'). By default it is added.
 *************************************************************/
void Commands::mode(void)
{
	if (_params.size() < 1)
	{
		return; /*ERR_NEEDMOREPARAMS;*/
	}

	Channel *channel = _server->findChannel(_params[1]);
	if (!channel)
	{
		return; /* ERR_NOSUCHCHANNEL */
	}

	bool isChanOper = channel->isOper(_user->getNickName());
	if (isChanOper == false)
		return; /* ERR_CHANOPRIVSNEEDED */

	bool remove = _params[2].find('-') ? true : false;
	char sign = remove == true ? '-' : '+';
	std::string modes = _params[2];
	modes.erase(std::remove(modes.begin(), modes.end(), sign), modes.end());

	std::string params = _params[3];
	for (size_t i(0); i < modes.size(); ++i)
		channel->modifyModes(modes[i], params, remove);
}

/*************************************************************
 * If a topic name is given: changes the topic name of a channel,
 *	or adds one if no topic has been set
 * If no topic name is given, prints the current topic name
 *************************************************************/
void Commands::topic(void)
{
	if (_params.size() < 2)
	{
		return; /*ERR_NEEDMOREPARAMS;*/
	}
	if (_params.size() == 2)
	{
		return; /* print topic name */
	}

	Channel *channel = _server->findChannel(_params[1]);

	if (!channel)
	{
		return; /* ERR_NOSUCHCHANNEL */
	}
	std::string newTopic = _params[2];

	if (channel->isTopicProtected() == false || _user->isOperator() == true)
		channel->setTopic(newTopic);
	else
	{
		return; /* ERR_CHANOPRIVSNEEDED */
	}
}

/*************************************************************
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
		std::vector<Channel *> channels = _server->getChannels();

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

	if (_params.size() > 1)
	{
		std::vector<std::string> channelNames = ircSplit(_params[1], ',');
		for (size_t i(0); i < channelNames.size(); ++i)
			if (Channel *chan = _server->findChannel(channelNames[i]))
				channels.push_back(chan);
	}
	else
		std::vector<Channel *> channels = _server->getChannels();

	for (size_t i(0); i < channels.size(); ++i)
	{

		if (!(channels[i]->isTopicProtected() || channels[i]->isPrivate()))
		{
			std::cout << channels[i]->getName() << std::endl;  // replace print fct
			std::cout << channels[i]->getTopic() << std::endl; // replace print fct
		}
	}
}

void Commands::invite(void)
{
	if (_params.size() < 4)
	{
		return; /*ERR_NEEDMOREPARAMS;*/
	}

	std::string nick = _params[1];
	if (!_server->findUserByNick(nick))
	{
		return; /* _ERRNOSUCHNICK */
	}
	Channel *channel = _server->findChannel(_params[2]);

	if (channel->isInviteOnly() && channel->isOper(nick) == false)
		return; /* ERR_CHANOPRIVSNEEDED */
	channel->join(_server->findUserByNick(nick));
	// handle err_useronchannel ?
}

/*************************************************************
 * Only a channel operator can kick out a user from the channel
 *************************************************************/
void Commands::kick(void)
{
	if (_params.size() < 3)
	{
		return; /*ERR_NEEDMOREPARAMS;*/
	}

	User *target = _server->findUserByNick(_params[2]);
	std::string user = _user->getNickName();
	Channel *channel = _server->findChannel(_params[1]);

	if (!channel)
	{
		return; /* ERR_NOSUCHCHANNEL */
	}
	if (channel->isOper(user) == false)
		return; /* ERR_CHANOPRIVSNEEDED */

	channel->part(target);

	if (_params.size() > 3)
	{
		std::string comment = _params[3];
		std::cout << comment << std::endl; // replace print fct
	}
	// handle err_notonchannel ?
}

void Commands::privmsg(void)
{
	std::vector<std::string> userNames = ircSplit(_params[1], ',');
	std::string message = _params[2];

	if (_params.size() < 2)
	{
		return; /*ERR_NORECIPIENT;*/
	}

	for (size_t i(0); i < userNames.size(); ++i)
	{
		User *target = _server->findUserByNick(userNames[i]);
		if (!target)
		{ /* ERR_NOSUCHNICK */
			return;
		}
		_server->sendMsg(target->getFd(), message);
	}
}

/*************************************************************
 * Unlike privmsg, does not send errorback error messages
 *************************************************************/
void Commands::notice(void)
{
	std::string message = _params[2];

	User *target = _server->findUserByNick(_params[1]);
	_server->sendMsg(target->getFd(), message);
}

void Commands::kill(void)
{
	if (_params.size() < 3)
	{
		return; /*ERR_NEEDMOREPARAMS;*/
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

	std::string msg = QUIT + _params[1];
	Commands cmd(_server, target, msg);
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
