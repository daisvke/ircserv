/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:50:56 by lchan             #+#    #+#             */
/*   Updated: 2022/11/15 18:23:22 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <exception>
# include <stdexcept>
# include <typeinfo>
# include <algorithm>
# include <bits/stdc++.h>
# include <signal.h>
# include <poll.h>

# define LOCAL_HOST "127.0.0.1"
# define SERVER_PORT 8084
# define BUFFER_SIZE 1024

# define ERROR -1
# define POLL_DELAY 5000

// # define TRUE 0
// # define ERROR -1 


# define MAX_CLIENT 100

# define ACCEPTED "the server has accepted your connection"
# define SERVER_START_MESS "Server is ready to receive connection"

#endif
