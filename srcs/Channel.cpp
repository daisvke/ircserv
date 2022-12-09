/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 05:34:57 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/21 07:40:43 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string key)
	: _name(name), _topic(), _modes("+"), _key(key), _userLimit()
{
		if (key.empty() == false) { _modes += "k"; }
}

Channel::~Channel() {}

/*************************************************************
 * Setters
 *************************************************************/

void Channel::setTopic(std::string name) { _topic = name; }

void Channel::modifyTargetMode(char c, std::string target, char sign)
{
	userDirectory::iterator it = _users.begin();

	while (it != _users.end())
	{
		if ((*it).first->getNickName() == target)
		{
			std::string *mode = &((*it).second);
			if (sign == '+')
				*mode += c;
			else
				mode->erase(c);
		}
		++it;
	}
}

void Channel::setChannelMode(char c, std::string params)
{
	switch (c)
	{
		case 'l':
			_userLimit = atoi(params.c_str());
			break;
		case 'k':
			_key = params;
	}
	_modes += c;
}

/*************************************************************
 * Getters
 *************************************************************/

std::string		Channel::getName(void) const { return _name; }
std::string 	Channel::getTopic(void) const { return _topic; }
std::string 	Channel::getModes(void) const { return _modes; }
userDirectory	*Channel::getUserDirectory(void) { return &_users; }
std::string		Channel::getKey(void) const { return _key; }
size_t			Channel::countUsers(void) const { return _users.size(); }
size_t			Channel::getUserLimit(void) const { return _userLimit; }

std::string *Channel::getUserMode(std::string name)
{
	userDirectory::iterator it = _users.begin();

	for (; it != _users.end(); ++it)
		if ((*it).first->getNickName() == name)
			return &(*it).second;
	return 0;
}

bool	Channel::isKeyProtected(void) const { return checkMode('k'); }
bool	Channel::isTopicProtected(void) const { return checkMode('t'); }
bool	Channel::isModerated(void) const { return checkMode('m'); }
bool	Channel::isInviteOnly(void) const { return checkMode('i'); }
bool	Channel::isSecret(void) const { return checkMode('s'); }
bool	Channel::isInternalOnly(void) const { return checkMode('n'); }
bool	Channel::isLimited(void) const { return checkMode('l'); }
bool	Channel::isEmpty(void) const { return _users.size() == 0; }

bool	Channel::isOper(std::string name) {
	return getUserMode(name)->find('o') == std::string::npos ? false : true;
}
bool	Channel::hasVoice(std::string name) {
	return getUserMode(name)->find('v') == std::string::npos ? false : true;
}

bool	Channel::isMember(std::string nick) {
	for (userDirectory::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (((*it).first->getNickName() == nick))
			return true;
	}
	return false;
}



/*************************************************************
 * Commands
 *************************************************************/

/*************************************************************
 * Add user to the user directory of the channel with its mode.
 * User is oper on the channel if:
 * 1. user is a global oper	2. user has created the channel
 *************************************************************/
void Channel::join(User *user, bool isOp)
{
	std::string	mode;
	if (isOp == true || user->isOperator()) mode = "o";
	_users[user] += mode;
}

/*************************************************************
 * Delete user from the user directory of the channel
 *************************************************************/
void Channel::part(User *user)
{
	for (userDirectory::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (((*it).first->getNickName() == user->getNickName()))
		{
			_users.erase(it);
			std::cout << user->getNickName() << " has parted channel " << getName() << std::endl;
			return ;
		}
	}
}

/*************************************************************
 * Prints all members of the channel
 *************************************************************/
void Channel::names(void)
{
	userDirectory::iterator it = _users.begin();

	for (; it != _users.end(); ++it)
		std::cout << (*it).first->getNickName() << std::endl; // replace print fct
}

/*************************************************************
 * Modifies user/channel modes.
 * If the mode is not already set, and remove is false,
 * 	then the new mode is added.
 * If the mode is already set, and remove is true,
 *	then the mode is removed.
 *************************************************************/
void Channel::modifyModes(char c, std::string params, char sign)
{
//	std::string channelModes = "lktmnspib";
	std::string channelModes = _CHANMODES;

	// User modes
	if (c == 'o' || c == 'v')
		modifyTargetMode(c, params, sign);
	else
	{
		// Channel modes
		if (sign == '+')
			setChannelMode(c, params);
		else if (sign == '-')
			_modes.erase(std::remove(_modes.begin(), _modes.end(), c), _modes.end());
	}
}

bool Channel::checkMode(char c) const
{
	if (_modes.find(c) != std::string::npos)
		return _FOUND;
	return _NOT_FOUND;
}
