/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:55:19 by lchan             #+#    #+#             */
/*   Updated: 2022/12/06 13:50:18 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "server.hpp"
#include <typeinfo>

bool	checkPort(std::string port)
{
	int	tmp = -1;
	
	if (!port.empty() && ircIsAlNum(port) == E_TRUE)
		tmp = ircStoi(port);
	if (tmp >=	0 && tmp <= USHRT_MAX)
		return (E_TRUE);
	return (E_FALSE);
}

bool	checkPassword(std::string pwd){

	if (!pwd.empty() && ircIsAllPrintable(pwd) == E_TRUE)
		return (E_TRUE);
	return (E_FALSE);
}

void	checkArgs(int ac, char **av){

	if (ac < 2 || ! av[1] || !av[2])
		throw std::invalid_argument("expecting ./ircserv <port> <password>");
	else if (checkPort(av[1]))
		throw std::invalid_argument("expected port 0 - 65535");
	else if (checkPassword(av[2]))
		throw std::invalid_argument("invalid a password");
}

int main(int ac, char **av)
{
	try{
		checkArgs(ac, av);
	}
	catch (std::exception &e){
		std::cout << "invalid_argument : " << e.what() << std::endl;
		return (0);
	}
	Server	Server(ircStoi(av[1]), av[2]);

	Server.startServer();
}

