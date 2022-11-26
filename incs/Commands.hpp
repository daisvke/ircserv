/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 05:54:10 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/21 07:39:52 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <vector>

# include "server.hpp"
# include "User.hpp"
# include "Channel.hpp"

enum	e_commands
{
	_NICK,
	_USER,
	_OPER,
	_QUIT,
	_JOIN,
	_PART,
	_TOPIC,
	_NAMES,
	_LIST,
	_INVITE,
	_KICK
};

/*************************************************************
* Contains the message sent by the user.
* The 'params' variable contains the whole input, including
* 	the command and the parameters
*************************************************************/
typedef struct s_message
{
	int							cmd;
	std::vector<std::string>	params;
}	t_message;


class Commands
{
	public:

	Commands(Server *server, User *user, t_message msg);
	~Commands();

	void	nick(void);
	void	user(void);
	void	oper(void);
	void	quit(void);
	void	join(void);
	void	part(void);
	void	topic(void);
	void	names(void) const;
	void	list(void) const;
	void	invite(void);
	void	kick(void);


	private:
	
		void		routeCmd();

		Server		*_server;
		User		*_user;
		t_message	_message;
};

#endif
