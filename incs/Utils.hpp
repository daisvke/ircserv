/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:52:50 by lchan             #+#    #+#             */
/*   Updated: 2022/11/29 14:08:41 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP


# include <stdlib.h>
# include <stddef.h>
# include <string>
# include <vector>
# include <sstream>

void						*ircMemset(void *s, int c, size_t n);
std::vector<std::string>	ircSplit(std::string str, char c);
std::string					ircTime();

#endif
