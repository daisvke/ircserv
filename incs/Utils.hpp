/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:52:50 by lchan             #+#    #+#             */
/*   Updated: 2022/12/06 13:32:58 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP


# include <stdlib.h>
# include <stddef.h>
# include <string>
# include <vector>
# include <sstream>
# include <iostream>

enum e_return{
	E_TRUE = 0,
	E_FALSE = 1
};

int	                    	setToTrue(bool	*res);
int							ircStoi(std::string input);
bool						ircIsAlNum(std::string &s);
bool						ircIsAllPrintable(std::string &s);
void						*ircMemset(void *s, int c, size_t n);
std::vector<std::string>	ircSplit(std::string str, char c);
std::string					ircTime(void);
std::string					getTimeStr(void);
std::string					concatArrayStrs(std::vector<std::string> array, size_t index);

template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif
