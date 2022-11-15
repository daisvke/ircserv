/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:55:19 by lchan             #+#    #+#             */
/*   Updated: 2022/11/14 19:03:01 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/headers.hpp"
#include "../incs/server.hpp"

// #include "headers.hpp"
// #include "server.hpp"
int main(){
	Server	Server;
	try {
			Server.initServer();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
}

