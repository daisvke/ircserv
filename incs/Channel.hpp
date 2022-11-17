#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <iostream>

# include "User.hpp"

// a map including a pointer to the users and indicating
// whether the user is an operator
typedef typename std::map<User *, std::string>	userDirectory;

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
		userDirectory	_users;
};

#endif
