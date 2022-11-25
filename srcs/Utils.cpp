/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:51:53 by lchan             #+#    #+#             */
/*   Updated: 2022/11/17 12:58:48 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

void	*IrcMemset(void *s, int c, size_t n)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		*(str + i) = c;
		++i;
	}
	return (s);
}

std::vector<std::string>	ircSplit(std::string str, char c)
{
	std::stringstream			input(str);
	std::string					segment;
	std::vector<std::string>	segmentVect;

	while (std::getline(input, segment, c))
		segmentVect.push_back(segment);
	return segmentVect;
}