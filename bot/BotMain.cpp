/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotMain.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 14:33:50 by lchan             #+#    #+#             */
/*   Updated: 2022/12/08 16:09:46 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "BotUtils.hpp"

bool	checkPort(std::string port)
{
	int	tmp;

	if (port.empty())
		throw std::invalid_argument("port is needed");
	else if (ircIsAlNum(port) == E_FALSE)
		throw std::invalid_argument("unexpected char in port");
	tmp = ircStoi(port);
	if (tmp < 1025 || tmp > USHRT_MAX)
		throw std::invalid_argument("expecting port between 1025 - 65535");
	return (E_FALSE);
}

void	checkPassword(std::string pwd){

	if (pwd.empty())
		throw std::invalid_argument("password is needed");
	else if (pwd.size() > MAX_PWD)
		throw std::invalid_argument("password is too long");
	else if (ircIsAllPrintable(pwd) == E_FALSE)
		throw std::invalid_argument("invalid char in password");
}

void	checkArgs(int ac, char **av){

	if (ac < 2 || ! av[1] || !av[2])
		throw std::invalid_argument("expecting ./ircserv_bot <port> <password>");
	checkPort(av[1]);
	checkPassword(av[2]);
}

int	main(int ac, char ** av){

	try{
		checkArgs(ac, av);
	}
	catch (std::exception &e){
		std::cout << "invalid_argument : " << e.what() << std::endl;
		return (0);
	}
	Bot	b(ircStoi(av[1]), av[2]);
}
