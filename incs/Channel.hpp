#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <iostream>

# include "User.hpp"

// modes
# define OP	1

// a map including a pointer to the users and indicating
// whether the user is an operator or a regular user on this channel
typedef typename std::map<User *, int>	userDirectory;

class Channel
{
	public:

		Channel(std::string name, std::string key);
		~Channel();

		// getters
		std::string		getName(void) const;
		userDirectory	getUserDirectory(void) const;

		// setters
		void	setTopic(std::string name);

		void	join(User *user);
		void	part(User *user);

	private:

		std::string		_name;
		std::string		_topic;
		int				_modes;
		userDirectory	_users;
		int				_userLimit;
};

#endif
