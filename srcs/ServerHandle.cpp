/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandle.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 18:58:55 by lchan             #+#    #+#             */
/*   Updated: 2022/11/29 20:47:04 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


// template <typename T>
// void	printMap(std::map<int, T> & mymap){
// 	for (mymap::iterator it = mymap.begin(); it!=mymap.end(); ++it)
// 		std::cout << mymap.first << " - " << mymap.second << std::endl;
// }

typedef std::pair<userMap::iterator,bool> userMapPair;
//std::pair<std::map<char,int>::iterator,bool> ret;
void	Server::handleCmd(int fd){

	std::pair<userMap::iterator,bool> userMapRet;
	std::pair<cmdMap::iterator,bool> cmdMapret;

	userMapRet = _userMap.insert(std::pair<int, User *>(fd, new User));
	if (userMapRet.second == true )
		_userMap[fd]->setFd(fd);
	cmdMapret = _cmdMap.insert(std::pair<int, std::string>(fd, _buffer));
	if (cmdMapret.second == true )
		_cmdMap[fd] += _buffer;
	// printMap(_cmdMap);
	// printMap(_userMap);
}

	// userMap::iterator it = _userMap.find('b');
	// if ( it == _userMap.end())
	// 	_userMap.insert(std::pair<int, User *>(fd, new User));

			// printf("_buffer = %s, recvRet = %d\n",_buffer, recvRet);
		// printf("_buffer[ret] = %d \n",_buffer[recvRet]);
		// printf("_buffer[ret - 1] = %d\n", _buffer[recvRet - 1]);
		// printf("_buffer[ret - 2] = %d \n", _buffer[recvRet - 2]);
		// printf("_buffer[ret - 3] = %d \n\n", _buffer[recvRet - 3]);
