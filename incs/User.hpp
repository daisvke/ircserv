/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 18:47:16 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/12/12 17:01:30 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
 
# include <vector>
# include <string>
# include <iostream>
# include <algorithm>
 
# include "main.hpp"
# include "Utils.hpp"

class User
{
	public:

		User();
		~User();

		// setters
		void	setFd(int fd);
		void	setId(std::string id);
		void	setAsPwdVerified(void);
		void	setAsRegistered(void);
		void	setNickName(std::string nick);
		void	setUserName(std::string user);
		void	setHostName(std::string host);
		void	setRealName(std::string real);
		void	setAsOperator(void);
		void	addChanInvitation(std::string channelName);
		void	modifyMode(char c, char sign);

		// getters
		int			getFd(void) const;
		std::string	getId(void) const;
		std::string getNickName(void) const;
		std::string getUserName(void) const;
		std::string getHostName(void) const;
		std::string getRealName(void) const;

		bool		isOperator(void) const;
		bool		isInvisible(void) const;
		bool		isPwdVerified(void) const;
		bool		isRegistered(void) const;
		bool		isInvited(std::string channelName) const;

		void		useInvitation(std::string channelName);


	private:

		int							_fd;
		bool 						_isPwdVerified;
		bool 						_isRegistered;
		std::string					_modes;
		std::string 				_id;
		std::string 				_nickName;
		std::string 				_userName;
		std::string 				_hostName;
		std::string 				_realName;
		std::vector<std::string>	_channelInvitations;
};

/* debug overload to delete*/
std::ostream &operator<<(std::ostream &o, User *const &rhs);

#endif
