/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:55:19 by lchan             #+#    #+#             */
/*   Updated: 2022/12/12 12:09:57 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "Server.hpp"
#include <typeinfo>

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
		throw std::invalid_argument("expecting ./ircserv <port> <password>");
	checkPort(av[1]);
	checkPassword(av[2]);
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


/*************** to do list 

/set dcc_autoaccept_lowports ON
/set dcc_autoget ON

/set dcc_download_path "/home/mint/Documents/42/ircserv/dcc_public"
/set dcc_file_create_mode 644
/set dcc_upload_path "/home/mint/Documents/42/ircserv/dcc_public"

/set dcc_upload_path "/mnt/nfs/homes/lchan/Documents/projet/ft_irc/dcc_public"

/dcc send u1 "/home/mint/Documents/42/ircserv/dcc_public/coucou.txt"

*/

//dcc send irssiLuc2 "/Documents/coucou.txt"

//dcc get irssiLuc "coucou.txt"