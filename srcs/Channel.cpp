/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 05:34:57 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/17 06:35:29 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name): _name(name) {}
Channel::~Channel() {}

// getters

std::string		Channel::getName(void) const { return _name; };
userDirectory	Channel::getUserDirectory(void) const { return _users; }


void	Channel::join(User *user)
{
	if (user->isOperator() || _users.empty()) {
		_users[user] += "+o";
	}
	std::cout << user->getNickName() << " has joined channel"
		<< getName() << "!" << std::endl;
}

void	Channel::part(User *user)
{
	_users.erase(user);
	std::cout << user->getNickName() << " has parted channel"
		<< getName() << "!" << std::endl;
}
