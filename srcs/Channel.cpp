/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 05:34:57 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/17 08:33:55 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string key): _name(name), _userLimit() {}
Channel::~Channel() {}

// getters

std::string		Channel::getName(void) const { return _name; };
userDirectory	Channel::getUserDirectory(void) const { return _users; }

void	Channel::setTopic(std::string name) { _topic = name; }

// add user to the user directory of the channel with its mode
void	Channel::join(User *user)
{
	if (user->isOperator() || _users.empty()) {
		_users[user] |= OP;
	}
	else
		_users[user] = 0;
	std::cout << user->getNickName() << " has joined channel"
		<< getName() << "!" << std::endl;
}

// delete user from the user directory of the channel
void	Channel::part(User *user)
{
	_users.erase(user);
	std::cout << user->getNickName() << " has parted channel"
		<< getName() << "!" << std::endl;
}
