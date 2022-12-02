/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandle.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 18:58:55 by lchan             #+#    #+#             */
/*   Updated: 2022/12/02 15:30:24 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/* debug function */
template <typename T>
void	printMap(std::map<int, T> & mymap){
	for (typename std::map<int, T>::iterator it = mymap.begin(); it!=mymap.end(); ++it){
		std::cout << "key : " << "[" << it->first << "]" << " - " << "val : "<< "[" << it->second << "]" << std::endl;
	}
}

void	Server::updateServerMaps(int key){

	std::pair<userMap::iterator,bool>	userMapRet;
	std::pair<cmdMap::iterator,bool>	cmdMapret;

	userMapRet = _userMap.insert(std::pair<int, User *>(key, new User));
	if (userMapRet.second == true)
		_userMap[key]->setFd(key);
	cmdMapret = _cmdMap.insert(std::pair<int, std::string>(key, _buffer));
	if (cmdMapret.second == false)
		_cmdMap[key] += _buffer;
}

std::string	extractCmd(std::string &s){

	std::string sub = "";
	std::size_t pos = s.find("\r\n");

	if (pos != std::string::npos)
	{
		sub = s.substr(0 , pos);
		s.erase (0, pos + 2 );
	}
	return (sub);
}

void	Server::execCmd(int key, std::string &str){

	Commands cmd = Commands(this, _userMap[key], str);
	//sendMsg(key, cmd.getRpl());
}

void	Server::cmdMaker(int key){

	std::string	cmd;

	while (1){
		cmd = extractCmd(_cmdMap[key]);
		if (cmd.empty())
			break;
		execCmd(key, cmd);
	}
}

/* key = _fds[index].fd */
void	Server::handleCmd(int key){

	updateServerMaps(key);
	cmdMaker(key);
	//printMap(_userMap);
}

void	Server::PrintInfo(void) {

	std::cout << "printing _userMap" << std::endl;
	printMap(_userMap);
	std::cout << "printing _cmdMap" << std::endl;
	printMap(_cmdMap);
}
