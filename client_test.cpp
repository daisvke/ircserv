/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:51:40 by lchan             #+#    #+#             */
/*   Updated: 2022/11/08 14:56:18 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"

# define CLIENT_MESS "this is a message from the client"

void	client(){

	struct sockaddr_in	addrClient;
	int					socketClient;
	int					newSocket;
	int					valread;
	int					addrlen = sizeof(addrClient);
	int					opt = 1;
	char				buffer[BUFFER_SIZE] = { 0 };

	if ((socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
		throw std::invalid_argument("client socket fail");
	else
		std::cout << "socket success" << std::endl;
 	if (setsockopt(socketClient, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		throw std::invalid_argument("client setsockopt fail");
	else
		std::cout << "socketopt success" << std::endl;
	addrClient.sin_family = AF_INET;
	addrClient.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	addrClient.sin_port = htons(PORT);

	if (connect(socketClient, (sockaddr *) &addrClient, sizeof(addrClient)) == SOCKET_ERROR)
		throw std::invalid_argument("client connect fail");
	else
		std::cout << "now connected" << std::endl;


	std::cin.getline(buffer, sizeof(buffer));
	std::cout << buffer << std::endl;
	send(socketClient, buffer, sizeof(buffer), 0);
	send(socketClient, CLIENT_MESS, strlen(CLIENT_MESS), 0);


	valread = read(socketClient, buffer, 1024);
	std::cout << "client buffer contains : [" << buffer << "]" << std::endl;

	close (socketClient);
}

int main()
{
	try {
		client();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
}
