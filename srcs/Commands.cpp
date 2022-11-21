/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:12 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/21 07:03:38 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

Commands::Commands(Server *server, User *user) : _server(server), _user(user) {};
Commands::~Commands() {};

void	Commands::nick(void)
{
	// need here to get the new nick from the cmd
	std::string	newNick;
	if (newNick.size() <= 1) { /*ERR_NONICKNAMEGIVEN;*/ }
	// need to check here if nick is already given among clients on the server
	_user->setNickName(newNick);

}
