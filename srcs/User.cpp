/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 18:47:19 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/17 06:43:14 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(): _isOper(false),_isConnected(false) {}
User::~User() {}

// setters

void	User::setFd(int fd) { _fd = fd; }
void	User::setNickName(std::string nick) { _nickName = nick; }
void	User::setUserName(std::string user) { _userName = user; }
void	User::setAsOperator(void) { _isOper = true; }
void	User::connect(void) { _isConnected = true; }
void	User::disconnect(void) { _isConnected = false; }

// getters

std::string	User::getNickName(void) const { return _nickName; }
std::string	User::getUserName(void) const { return _userName; }
bool		User::isOperator(void) const { return _isOper; }
bool		User::isConnected(void) const { return _isConnected; }

