/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:12 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/30 20:50:08 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "iterator"

Commands::Commands(Server *server, User *user, t_message msg)
	: _server(server), _user(user), _message(msg) { routeCmd(); }

Commands::Commands(Server *server, User *user, std::string &str)
	: _server(server), _user(user), _params(ircSplit(str, ' '))
	{
		_message.params = ircSplit(str, ' ');
		// If corresponds to unused line thrown before the first command,
		//	skip it.
		if (_params[0] == "CAP")
			return ;
		setupMap();
	}

Commands::~Commands() {}

void	Commands::routeCmd()
{
	switch (_message.cmd)
	{
		case _NICK:		nick(); break;
		case _USER:		user(); break;
		case _OPER:		oper(); break;
		case _QUIT:		quit(); break;
		case _JOIN:		join(); break;
		case _PART:		part(); break;
		case _MODE:		mode(); break;
		case _TOPIC:	topic(); break;
		case _NAMES:	names(); break;
		case _LIST:		list(); break;
		case _INVITE:	invite(); break;
		case _KICK:		kick(); break;
		case _KILL:		kill(); break;

		default:		std::cerr << "erroooor cmd not found" << std::endl; //replace fct
	}
}

template <typename T>
void	printMap(std::map<std::string, T> & mymap){
	for (typename std::map<std::string, T>::iterator it = mymap.begin(); it!=mymap.end(); ++it){
		std::cout << "key : " <<it->first << " - "<< "val : "<<it->second << std::endl;
	}
}

void	Commands::setupMap()
{
	_cmdMap[NICK]	=		&Commands::nick;
	_cmdMap[USER]	=		&Commands::user;
	_cmdMap[OPER]	=		&Commands::oper;
	_cmdMap[QUIT]	=		&Commands::quit;
	_cmdMap[JOIN]	=		&Commands::join;
	_cmdMap[PART]	=		&Commands::part;
	_cmdMap[MODE]	=		&Commands::mode;
	_cmdMap[TOPIC]	=		&Commands::topic;
	_cmdMap[NAMES]	=		&Commands::names;
	_cmdMap[LIST]	=		&Commands::list;
	_cmdMap[INVITE]	=		&Commands::invite;
	_cmdMap[KICK]	=		&Commands::kick;
	_cmdMap[KILL]	=		&Commands::kill;

	cmdMap::iterator it;
	it = _cmdMap.find(_params[0]);
	if (it != _cmdMap.end())
		(this->*_cmdMap[_params[0]])();
	// else
	// 	std::cout << "ERROR IS HERE \n" << std::endl;

	// std::cout << "inside setupMaps" << std::endl;
	// std::cout << "_cmdMap[_params[0]] = " << _cmdMap[_params[0]] << std::endl;
	// std::cout << "_cmdMap[_params[1]] = " << _cmdMap[_params[1]] << std::endl;
	// for (size_t i(0); i < _params.size(); ++i)
	// 		std::cout << i << ":" <<_params[i] << std::endl;
}

/*************************************************************
* Changes the user's nickname online; for example /nick Carlos
*************************************************************/
void	Commands::nick(void)
{
	std::cout << " >>>>>>>>>> inside nick function" << std::endl;
	if (_params.size() < 2) { printf("size problem !!!!!! \n"); return ;/* ERR_NONICKNAMEGIVEN */ }

	std::string	newNick = _params[1];
	if (_server->findUserByNick(newNick) == false)
		_user->setNickName(newNick);
	else {
		// return ERR_NICKNAMEINUSE
	}
	std::cout << " >>>>>>>>>> outside nick function" << std::endl;

}

/*************************************************************
* Used at the begining of a connection
*************************************************************/
void	Commands::user(void)
{
	std::cout << " >>>>>>>>>> inside user function" << std::endl;

	if (_message.params.size() < 2) { return ;/*ERR_NEEDMOREPARAMS */}

	std::string	newUserName = _message.params[1];
	if (_server->findUserByName(newUserName) == false)
		_user->setUserName(newUserName);
	else { /* ERR_ALREADYREGISTRED */ }
	std::cout << "outside user function <<<<<<<<<<<<<<" << std::endl;

}

/*************************************************************
* Used by a user to get oper privileges
*************************************************************/
void	Commands::oper(void)
{
	if (_message.params.size() < 3) { return ; /*ERR_NEEDMOREPARAMS */}

	User		*user = _server->findUserByNick(_message.params[1]);
	if (!user) { return ;/* ERR not found */ }
	if (_server->getPassword() != _message.params[1]) {
		return; /* ERR_PASSWDMISMATCH */
	}
	_user->setAsOperator();
}

/*************************************************************
* The user quits the network, which makes him leave all the channels as well
*************************************************************/
void	Commands::quit(void)
{
	std::string				lastWords = _message.params[1];
	std::vector<Channel *>	channels = _server->getChannels();

	_user->disconnect();
	for (size_t i(0); i < channels.size(); ++i)
	{
		userDirectory			users = channels[i]->getUserDirectory();
		userDirectory::iterator	it = users.begin();

		for (; it != users.end(); ++it)
			if ((*it).first->getNickName() == _user->getNickName())
				channels[i]->part((*it).first);
	}
	if (lastWords.empty() == false)
		std::cout << lastWords << std::endl; // replace by server printer function
}

/*************************************************************
* The user begins listening to a channel
*************************************************************/
void	Commands::join(void)
{
	if (_message.params.size() < 2) { return ;/*ERR_NEEDMOREPARAMS;*/ }

	std::vector<std::string>	channelNames = ircSplit(_message.params[1], ',');
	std::vector<std::string>	channelKeys = ircSplit(_message.params[2], ',');

	for (size_t i(0); i < channelNames.size(); ++i) {
		Channel	*channel = _server->findChannel(channelNames[i]);
		if (!channel) { continue ; /* ERR_NOSUCHCHANNEL */ }
		if (channel->isKeyProtected())
			if (channelKeys[i] != channel->getKey())
				return ; /* ERR_BADCHANNELKEY */
		if (channel->getUserNbr() < channel->getUserLimit())
			channel->join(_user);
		else { /* ERR_CHANNELISFULL */ }
	}

	// handle? => user shouldn't be banned  ERR_BANNEDFROMCHAN
	// handle ? => user should be invited if channel is invite only
	//		ERR_INVITEONLYCHAN

	// when join succeeds, chan sends to user the topic name + lsit of
	// 	all users on the chan, with himself on the list as well
}

/*************************************************************
*  The user quits all the channels given as parameters
*************************************************************/
void	Commands::part(void)
{
	if (_message.params.size() < 2) { return ;/*ERR_NEEDMOREPARAMS;*/ }

	std::vector<std::string>	channelNames = ircSplit(_message.params[1], ',');

	for (size_t i(0); i < channelNames.size(); ++i) {
		Channel	*channel = _server->findChannel(channelNames[i]);

		if (!channel) { continue ; /* ERR_NOSUCHCHANNEL */ }
		userDirectory			users = channel->getUserDirectory();
		userDirectory::iterator	it = users.begin();

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
void	Commands::mode(void)
{
	if (_message.params.size() < 1) { return ;/*ERR_NEEDMOREPARAMS;*/ }

	Channel	*channel = _server->findChannel(_message.params[1]);
	if (!channel) { return ; /* ERR_NOSUCHCHANNEL */ }

 	bool	isChanOper = channel->isOper(_user->getNickName());
	if (isChanOper == false)
		return ; /* ERR_CHANOPRIVSNEEDED */

	bool	remove = _message.params[2].find('-') ? true : false;
	char	sign = remove == true ? '-' : '+';
	std::string	modes = _message.params[2];
	modes.erase(std::remove(modes.begin(), modes.end(), sign), modes.end());

	std::string	params = _message.params[3];
	for (size_t i(0); i < modes.size(); ++i)
		channel->modifyModes(modes[i], params, remove);
}

/*************************************************************
* If a topic name is given: changes the topic name of a channel,
*	or adds one if no topic has been set
* If no topic name is given, prints the current topic name
*************************************************************/
void	Commands::topic(void)
{
	if (_message.params.size() < 2) { return ;/*ERR_NEEDMOREPARAMS;*/ }
	if (_message.params.size() == 2) { return ; /* print topic name */ }

	Channel	*channel = _server->findChannel(_message.params[1]);

	if (!channel) { return ; /* ERR_NOSUCHCHANNEL */ }
	std::string	newTopic = _message.params[2];

	if (channel->isTopicProtected() == false
		|| _user->isOperator() == true)
		channel->setTopic(newTopic);
	else { return; /* ERR_CHANOPRIVSNEEDED */ }
}

/*************************************************************
* If no channel name is given as param:
*	all the channels with all its members are printed.
* If channel names are given:
* 	Prints all members of all the given non private, non secret, or currently listening channels.
* There is no error message for wrong channel names etc.
*************************************************************/
void	Commands::names(void)
{
	std::vector<Channel *>	channels;

	if (_message.params.size() > 1)
	{
		std::vector<std::string>	channelNames = ircSplit(_message.params[1], ',');
		for (size_t i(0); i < channelNames.size(); ++i)
			if (Channel	*chan = _server->findChannel(channelNames[i]))
				channels.push_back(chan);
	}
	else
		std::vector<Channel *>	channels = _server->getChannels();

	for (size_t i(0); i < channels.size(); ++i) {

		if (!(channels[i]->isTopicProtected() || channels[i]->isPrivate()))
		{
			std::cout << channels[i]->getName() << std::endl; // replace print fct
			channels[i]->names();
		}
	}
}

void	Commands::list(void)
{
	std::vector<Channel *>	channels;

	if (_message.params.size() > 1)
	{
		std::vector<std::string>	channelNames = ircSplit(_message.params[1], ',');
		for (size_t i(0); i < channelNames.size(); ++i)
			if (Channel	*chan = _server->findChannel(channelNames[i]))
				channels.push_back(chan);
	}
	else
		std::vector<Channel *>	channels = _server->getChannels();

	for (size_t i(0); i < channels.size(); ++i) {

		if (!(channels[i]->isTopicProtected() || channels[i]->isPrivate()))
		{
			std::cout << channels[i]->getName() << std::endl; // replace print fct
			std::cout << channels[i]->getTopic() << std::endl; // replace print fct
		}
	}
}

void	Commands::invite(void)
{
	if (_message.params.size() < 4) { return ;/*ERR_NEEDMOREPARAMS;*/ }

	std::string	nick = _message.params[1];
	if (!_server->findUserByNick(nick)) { return ; /* _ERRNOSUCHNICK */ }
	Channel		*channel = _server->findChannel(_message.params[2]);

	if (channel->isMembersOnly() && channel->isOper(nick) == false)
		return ; /* ERR_CHANOPRIVSNEEDED */
	channel->join(_server->findUserByNick(nick));
	// handle err_useronchannel ?
}

/*************************************************************
* Only a channel operator can kick out a user from the channel
*************************************************************/
void	Commands::kick(void)
{
	if (_message.params.size() < 3) { return ;/*ERR_NEEDMOREPARAMS;*/ }

	User		*target = _server->findUserByNick(_message.params[2]);
	std::string	user = _user->getNickName();
	Channel		*channel = _server->findChannel(_message.params[1]);

	if (!channel) { return ; /* ERR_NOSUCHCHANNEL */ }
	if (channel->isOper(user) == false)
		return ; /* ERR_CHANOPRIVSNEEDED */

	channel->part(target);

	if (_message.params.size() > 3)
	{
		std::string	comment = _message.params[3];
		std::cout << comment << std::endl; // replace print fct
	}
	// handle err_notonchannel ?
}

//void	Commands::privmsg(void) {}

//void	Commands::notice(void) {}

void	Commands::kill(void)
{
	if (_message.params.size() < 3) { return ;/*ERR_NEEDMOREPARAMS;*/ }
	if (_user->isOperator() == false) { return ; /*ERR_NOPRIVILEGES*/}

	User		*target = _server->findUserByNick(_message.params[1]);
	if (!target) { return ; /* ERR_NOSUCHNICK */ }

	std::vector<std::string>	comment; comment.push_back(_message.params[1]);
	t_message	msg = { _QUIT, comment};
	Commands	cmd(_server, target, msg);
}
