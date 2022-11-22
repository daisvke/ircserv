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

typedef struct s_message
{
	std::string					cmd;
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
	void	topic(void);
	void	names(void) const;
	void	list(void) const;

	private:
		Server		*_server;
		User		*_user;
		t_message	_message;
};

#endif
