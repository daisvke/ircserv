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
	: _name(name), _topic(), _key(key), _userLimit()
{
		if (key.empty() == false) { _modes += "k"; }

}

Channel::~Channel() {}

/*************************************************************
 * Setters
 *************************************************************/

void Channel::setTopic(std::string name) { _topic = name; }

void Channel::modifyTargetMode(char c, std::string target, bool remove)
{
	userDirectory::iterator it = _users.begin();

	while (it != _users.end())
	{
		if ((*it).first->getNickName() == target)
		{
			std::string *mode = &((*it).second);
			if (remove == true)
				*mode += c;
			else
				mode->erase(c);
		}
		++it;
	}
	std::cout << "Client name " << target << " not found!" << std::endl; // replace
}

void Channel::setChannelMode(char c, std::string params)
{
	std::string modes = "timnsp";
	if (modes.find(c) != std::string::npos)
	{
		_modes += c;
		return;
	};

	switch (c)
	{
	case 'l':
		_userLimit = atoi(params.c_str());
		break;
	case 'k':
		_key = params;
	}
}

/*************************************************************
 * Getters
 *************************************************************/

std::string Channel::getName(void) const { return _name; }
std::string Channel::getTopic(void) const { return _topic; }
userDirectory *Channel::getUserDirectory(void) { return &_users; }
std::string Channel::getKey(void) const { return _key; }
size_t Channel::getUserNbr(void) const { return _users.size(); }
size_t Channel::getUserLimit(void) const { return _userLimit; }

std::string *Channel::getUserMode(std::string name)
{
	userDirectory::iterator it = _users.begin();

	for (; it != _users.end(); ++it)
		if ((*it).first->getNickName() == name)
			return &(*it).second;
	return 0;
}

bool Channel::isKeyProtected(void) const { return checkMode('k'); }
bool Channel::isTopicProtected(void) const { return checkMode('t'); }
bool Channel::isModerated(void) const { return checkMode('m'); }
bool Channel::isInviteOnly(void) const { return checkMode('n'); }
bool Channel::isSecret(void) const { return checkMode('s'); }
bool Channel::isPrivate(void) const { return checkMode('p'); }
bool Channel::isLimited(void) const { return checkMode('l'); }

bool Channel::isOper(std::string name)
{
	return getUserMode(name)->find('o') ? true : false;
}

/*************************************************************
 * Commands
 *************************************************************/

/*************************************************************
 * Add user to the user directory of the channel with its mode.
 * User is oper on the channel if:
 * 1. user is a global oper	2. user has created the channel
 *************************************************************/
void Channel::join(User *user, bool isOper)
{
	if (isOper == true || (isOper == false && user->isOperator()))
	{
		std::string	mode = "o";
		_users[user] += mode;
	}
}

/*************************************************************
 * Delete user from the user directory of the channel
 *************************************************************/
void Channel::part(User *user)
{
	userDirectory::iterator it = _users.begin();

	for (; it != _users.end(); ++it)
		if (((*it).first->getNickName() == user->getNickName()))
			_users.erase(user);
	std::cout << user->getNickName() << " has parted channel '"
			  << getName() << "'!" << std::endl;
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
void Channel::modifyModes(char c, std::string params, bool remove)
{
	std::string channelModes = "lktmnspib";

	if (c == 'o' || c == 'v')
		modifyTargetMode(c, params, remove);

	if (channelModes.find(c) != std::string::npos)
		return; /* ERR_UNKNOWNMODE */
	if (_modes.find(c) != std::string::npos)
	{
		setChannelMode(c, params);
	}
	else if (remove == true)
	{
		_modes.erase(c);
	}

	// handle b ? n-v
}

bool Channel::checkMode(char c) const
{
	if (_modes.find(c) != std::string::npos)
		return true;
	return false;
}
