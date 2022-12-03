/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 18:47:16 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/29 22:56:41 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>

class User
{
	public:

		User();
		~User();

		// setters
		void	setFd(int fd);
		void	setNickName(std::string nick);
		void	setUserName(std::string user);
		void	setHostName(std::string host);
		void	setRealName(std::string real);
		void	setAsOperator(void);
		void	connect(void);
		void	disconnect(void);

		// getters
		int			getFd(void) const;
		std::string	getNickName(void) const;
		std::string	getUserName(void) const;
		std::string	getHostName(void) const;
		std::string	getRealName(void) const;

		bool		isOperator(void) const;
		bool		isConnected(void) const;

	private:

		int			_fd;
		bool		_isOper;
		bool		_isConnected; // usefull ?
		std::string	_nickName;
		std::string	_userName;
		std::string	_hostName;
		std::string	_realName;
};

/* debug overload to delete*/
std::ostream& operator<<(std::ostream& o,  User *const & rhs);

#endif
