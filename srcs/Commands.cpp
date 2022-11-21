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

void	Commands::topic(void)
{
	// need here a method to find a channel from all the channels
	Channel	*channel;
	std::string	newTopic; // need to get new topic name from cmd params

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
