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

User::User(): _isOper(false),_isConnected(false) {}
User::~User() {}

/*************************************************************
* Setters
*************************************************************/
void	User::setFd(int fd) { _fd = fd; }
void	User::setNickName(std::string nick) { _nickName = nick; }
void	User::setUserName(std::string user) { _userName = user; }
void	User::setAsOperator(void) { _isOper = true; }
void	User::connect(void) { _isConnected = true; }
void	User::disconnect(void) { _isConnected = false; }

/*************************************************************
* Getters
*************************************************************/
int			User::getFd(void) const {return _fd;}
std::string	User::getNickName(void) const { return _nickName; }
std::string	User::getUserName(void) const { return _userName; }
bool		User::isOperator(void) const { return _isOper; }
bool		User::isConnected(void) const { return _isConnected; }

/* debug overload to delete*/
std::ostream & operator<<(std::ostream & o, User * const &rhs){

	o	<< "_fd :" << rhs->getFd() << " "
		<< "_isOper: " << rhs->isOperator() << " "
		<< "_isConnected: " << rhs->isConnected() << " "
		<< "_nickName: " << rhs->getNickName() << " "
		<< "_userName: " << rhs->getUserName();
	return (o);
}
