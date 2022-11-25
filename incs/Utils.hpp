/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:52:50 by lchan             #+#    #+#             */
/*   Updated: 2022/11/17 12:57:31 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP


# include <stdlib.h>
# include <stddef.h>
# include <string>
# include <vector>
# include <sstream>

void	*IrcMemset(void *s, int c, size_t n);
std::vector<std::string>	ircSplit(std::string str, char c);

#endif
