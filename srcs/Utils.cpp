/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:51:53 by lchan             #+#    #+#             */
/*   Updated: 2022/12/06 13:32:39 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

int	 ircStoi(std::string input){

	std::stringstream	stream;
	int					i;

	i = 0;
	stream << input;
	stream >> i;
	return (i);
}

typedef	std::string::iterator strIt;

bool	ircIsAlNum(std::string &s)
{
	for (strIt it = s.begin(); it != s.end(); ++it)
		if (!std::isdigit(*it))
			return (E_FALSE);
	return (E_TRUE);
}

bool	ircIsAllPrintable(std::string &s)
{
	for (strIt it = s.begin(); it != s.end(); ++it)
		if (static_cast<char> (*it) < 32 && static_cast<char>(*it) > 126)
			return (E_FALSE);
	return (E_TRUE);
}


void	*ircMemset(void *s, int c, size_t n)
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

/* https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
struct tm {
   int tm_sec;   // seconds of minutes from 0 to 61
   int tm_min;   // minutes of hour from 0 to 59
   int tm_hour;  // hours of day from 0 to 24
   int tm_mday;  // day of month from 1 to 31
   int tm_mon;   // month of year from 0 to 11
   int tm_year;  // year since 1900
   int tm_wday;  // days since sunday
   int tm_yday;  // days since January 1st
   int tm_isdst; // hours of daylight savings time
}
*/

std::string	ircTime(){

	time_t				now = time(0);
	std::stringstream	ss;

	tm *ltm = localtime(&now);
	ss	<< ltm->tm_hour
		<< ":"
		<< ((ltm->tm_min < 10) ? "0" : "")
		<< ltm->tm_min << " ";
	return (ss.str());
}

std::string	getTimeStr(){

	time_t				now = time(0);
	std::string			dt = ctime(&now);

	dt.erase(dt.end() - 1); // erase the \n of the end

	return(dt);
}

std::string	concatArrayStrs(std::vector<std::string> array, size_t index)
{
	std::string	concatenatedStr = array[index];

	for (size_t i(index); i < array.size(); ++i)
		concatenatedStr += " " + array[i];
	return concatenatedStr;
}