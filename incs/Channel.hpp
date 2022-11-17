#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <iostream>

# include "User.hpp"

// modes
# define OP	1

// a map including a pointer to the users and indicating
// whether the user is an operator
typedef typename std::map<User *, int>	userDirectory;

class Channel
{
	public:

		Channel(std::string name);
		~Channel();

		// getters
		std::string		getName(void) const;
		userDirectory	getUserDirectory(void) const;

		void	join(User *user);
		void	part(User *user);

	private:

		std::string		_name;
		int				_modes;
		userDirectory	_users;
};

#endif
