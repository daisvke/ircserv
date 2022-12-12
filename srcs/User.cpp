/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 18:47:19 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/01 17:47:00 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _fd(), _isPwdVerified(false), _isRegistered(false), _isOper(false) {}
User::~User() {}

/*************************************************************
 * Setters
 *************************************************************/
void User::setFd(int fd) { _fd = fd; }
void User::setId(std::string id) { _id = id; }
void User::setAsPwdVerified(void) { _isPwdVerified = true; }
void User::setAsRegistered(void) { _isRegistered = true; }
void User::setNickName(std::string nick) { _nickName = nick; }
void User::setUserName(std::string user) { _userName = user; }
void User::setHostName(std::string host) { _hostName = host; }
void User::setRealName(std::string real) { _realName = real; }
void User::setAsOperator(void) { modifyMode('o', '+'); }

void User::modifyMode(char c, char sign)
{
	if (sign == '+')
		_modes += c;
	else
		_modes.erase(std::remove(_modes.begin(), _modes.end(), c), _modes.end());
}

void User::addChanInvitation(std::string channelName)
{
	_channelInvitations.push_back(channelName);
}

/*************************************************************
 * Getters
 *************************************************************/
int User::getFd(void) const { return _fd; }
std::string User::getId(void) const { return _id; }
std::string User::getNickName(void) const { return _nickName; }
std::string User::getUserName(void) const { return _userName; }
std::string User::getHostName(void) const { return _hostName; }
std::string User::getRealName(void) const { return _realName; }

bool User::isOperator(void) const { return checkMode(_modes, 'o'); }
bool User::isInvisible(void) const { return checkMode(_modes, 'i'); }
bool User::isPwdVerified(void) const { return _isPwdVerified; }
bool User::isRegistered(void) const { return _isRegistered; }

bool User::isInvited(std::string channelName) const
{
	for (size_t i(0); i < _channelInvitations.size(); ++i)
		if (channelName == _channelInvitations[i])
			return true;
	return false;
}

void User::useInvitation(std::string channelName)
{
	std::vector<std::string>::iterator it;
	for (it = _channelInvitations.begin(); it != _channelInvitations.end(); ++it)
		if (channelName == (*it))
		{
			_channelInvitations.erase(it);
			return;
		}
}

/* debug overload to delete*/
std::ostream &operator<<(std::ostream &o, User *const &rhs)
{

	o << "_fd :" << rhs->getFd() << " "
	  << "_isOper: " << rhs->isOperator() << " "
	  << "_nickName: " << rhs->getNickName() << " "
	  << "_userName: " << rhs->getUserName();
	return (o);
}
