/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandle.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 18:58:55 by lchan             #+#    #+#             */
/*   Updated: 2022/11/30 13:53:58 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


/* debug function */
template <typename T>
void	printMap(std::map<int, T> & mymap){
	for (typename std::map<int, T>::iterator it = mymap.begin(); it!=mymap.end(); ++it){
		std::cout << "key : " <<it->first << "\n" << "val : "<<it->second << std::endl;
	}
}

void	Server::updateServerMaps(int key){

	std::pair<userMap::iterator,bool> userMapRet;
	std::pair<cmdMap::iterator,bool> cmdMapret;

	userMapRet = _userMap.insert(std::pair<int, User *>(key, new User));
	if (userMapRet.second == true )
		_userMap[key]->setFd(key);
	cmdMapret = _cmdMap.insert(std::pair<int, std::string>(key, _buffer));
	if (cmdMapret.second == false )
		_cmdMap[key] += _buffer;
}

std::string	extractCmd(std::string &s){

	std::string sub = 0;
	std::size_t pos = s.find("\r\n");

	if (pos != std::string::npos)
	{
		sub = s.substr(0 , pos);
		s.erase (0, pos + 2 );
	}
	return (sub);
}

void	Server::execCmd(int key, std::string &str){

	Commands cmd = Commands(this, _userMap[key], _cmdMap[key]);
}

void	Server::cmdMaker(int key){

	std::string	cmd;

	while (1){
		cmd = extractCmd(_cmdMap[key]);
		if (cmd ==)
			break;
		execCmd(int key, cmd);
	}
}


void	Server::handleCmd(int key){

	updateServerMaps(key);
	cmdMaker(key);
}


	// userMap::iterator it = _userMap.find('b');
	// if ( it == _userMap.end())
	// 	_userMap.insert(std::pair<int, User *>(fd, new User));

			// printf("_buffer = %s, recvRet = %d\n",_buffer, recvRet);
		// printf("_buffer[ret] = %d \n",_buffer[recvRet]);
		// printf("_buffer[ret - 1] = %d\n", _buffer[recvRet - 1]);
		// printf("_buffer[ret - 2] = %d \n", _buffer[recvRet - 2]);
		// printf("_buffer[ret - 3] = %d \n\n", _buffer[recvRet - 3]);
