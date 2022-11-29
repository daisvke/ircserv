/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 18:47:16 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/11/29 20:03:52 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef USER_HPP
# define USER_HPP

# include <string>

class User
{
	public:

		User();
		~User();

		// setters
		void	setFd(int fd);
		void	setNickName(std::string nick);
		void	setUserName(std::string user);
		void	setAsOperator(void);
		void	connect(void);
		void	disconnect(void);

		// getters
		std::string	getNickName(void) const;
		std::string	getUserName(void) const;
		bool		isOperator(void) const;
		bool		isConnected(void) const;

	private:

		int			_fd;
		bool		_isOper;
		bool		_isConnected;
		std::string	_nickName;
		std::string	_userName;
};

#endif
