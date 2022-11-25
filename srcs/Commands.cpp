/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:12 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/21 07:44:59 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

Commands::Commands(Server *server, User *user, s_message msg)
	: _server(server), _user(user), _message(msg) {};

Commands::~Commands() {};

/*************************************************************
* Changes the user's nickname online; for example /nick Carlos
*************************************************************/
void	Commands::nick(void)
{
	if (_message.params.size() < 2) { /* ERR_NONICKNAMEGIVEN */ }

	std::string	newNick = _message.params[1];
	if (_server->findUser(newNick) == false)
		_user->setNickName(newNick);
	else {
		// return ERR_NICKNAMEINUSE
	}
}

/*************************************************************
* Used at the begining of a connection 
*************************************************************/
void	Commands::user(void)
{
	if (_message.params.size() < 2) { /*ERR_NEEDMOREPARAMS */}

	std::string	newUserName = _message.params[1];
	if (_server->findUser(newUserName) == false)
		_user->setUserName(newUserName);
	else { /* ERR_ALREADYREGISTRED */ }
}

/*************************************************************
* Used by a user to get oper privileges
*************************************************************/
void	Commands::oper(void)
{
	if (_message.params.size() < 3) { /*ERR_NEEDMOREPARAMS */}

	User		*user = _server->findUser(_message.params[1]);
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
	_user->disconnect();
	std::string	lastWords = _message.params[1];
	// remove from all channels
	std::vector<Channel *>	channels = _server->getChannels();
	for (size_t i(0); i < channels.size(); ++i)
	{
		userDirectory	users = channels[i]->getUserDirectory();
		userDirectory::iterator	it = users.begin();
		for (; it != users.end(); ++it)
			if ((*it).first->getNickName() == _user->getNickName()) { users.erase(it); }
	}
	if (lastWords.empty() == false)
	std::cout << lastWords << std::endl; // replace by server printer function
}

/*************************************************************
* The user begins listening to a channel
*************************************************************/
void	Commands::join(void)
{
	if (_message.params.size() < 2) { /*ERR_NEEDMOREPARAMS;*/ }
	
	std::vector<std::string>	channelNames = ircSplit(_message.params[1], ',');
	std::vector<std::string>	channelKeys = ircSplit(_message.params[2], ',');

	for (size_t i(0); i < channelNames.size(); ++i) {
		Channel	*channel = _server->findChannel(channelNames[i]);
		if (!channel) { continue ; /* ERR_NOSUCHCHANNEL */ }
		if (channel->isKeyProtected())
			if (channelKeys[i] != channel->getKey())
				return ; /* ERR_BADCHANNELKEY */
		channel->join(_user);
	}

	// handle? => user shouldn't be banned  ERR_BANNEDFROMCHAN
	// handle ? => user should be invited if channel is invite only
	//		ERR_INVITEONLYCHAN

	// when join succeeds, chan sends to user the topic name + lsit of
	// 	all users on the chan, with himself on the list as well
}

/*************************************************************
*  
*************************************************************/
void	Commands::topic(void)
{
	Channel	*channel;
	std::string	newTopic; // need to get new topic name from cmd params

	if (_message.params.size() < 1) { /*ERR_NONICKNAMEGIVEN;*/ }

	if (channel->isTopicProtected() == false)
		channel->setTopic(newTopic);
}

void	Commands::names(void) const
{
	// need here a method to find a channel from all the channels
	Channel	*channel;

	if (channel->isTopicProtected() || channel->isPrivate())
		return ;
}

void	Commands::list(void) const
{
	std::vector<Channel *>	channelVect;
	std::vector<Channel *>::iterator	it = channelVect.begin();

	// need to go throught the real array of channels here
	for (;it != channelVect.end(); ++it)
	{
		std::cout << (*it)->getName() << std::endl;
		std::cout << (*it)->getTopic() << std::endl; //to send to clients instead
	}
}
