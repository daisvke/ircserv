/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 05:34:57 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/18 05:16:33 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string key): _name(name), _userLimit() {}
Channel::~Channel() {}


/*************************************************************
* Setters
*************************************************************/

void	Channel::setTopic(std::string name) { _topic = name; }

void	Channel::setTargetMode(char c, std::string target)
{
	userDirectory::iterator	it = _users.begin();

	while (it != _users.end())
	{
		if ((*it).first->getNickName() == target)
		{
			std::string	*mode = &((*it).second);
			*mode += c;
		}
		++it;
	}
	std::cout << "Client name " << target << " not found!" << std::endl; 
}

void	Channel::setChannelMode(char c, std::string params)
{
	std::string	modes = "timnsp";
	if (modes.find(c)) { _mode += c; return ; };

	switch(c)
	{
		case 'l': _userLimit = atoi(params.c_str()); break ;
		case 'k': _key = params;
	}
}


/*************************************************************
* Getters
*************************************************************/

std::string		Channel::getName(void) const { return _name; };
userDirectory	Channel::getUserDirectory(void) const { return _users; }


/*************************************************************
* Commands
*************************************************************/

/*************************************************************
* Add user to the user directory of the channel with its mode.
* User is oper on the channel if:
* 1. user is a global oper	2. user has created the channel
*************************************************************/
void	Channel::join(User *user)
{
	if (user->isOperator() || _users.empty()) {
		_users[user] += "o";
	}
	else
		_users[user].clear();
	std::cout << user->getNickName() << " has joined channel '"
		<< getName() << "'!" << std::endl;
}

/*************************************************************
* Delete user from the user directory of the channel
*************************************************************/
void	Channel::part(User *user)
{
	_users.erase(user);
	std::cout << user->getNickName() << " has parted channel '"
		<< getName() << "'!" << std::endl;
}

/*************************************************************
* Interpret given commands			'b' on server.cpp ?
*************************************************************/
int	Channel::handleModes(char mode, std::string params)
{
	if (mode == 'o' || mode == 'v'/* || mode == 'b'*/)
		setTargetMode(mode, params);
	else if (mode == 'l' || mode == 'k') setChannelMode(mode, params);
	else
		return ERROR;
	return SUCCESS;
}
