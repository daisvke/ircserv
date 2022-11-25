#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <iostream>
# include <algorithm>
# include <cstring>

# include "User.hpp"

# define SUCCESS	0
# define ERROR		1

/*************************************************************
* A map including a pointer to the users and indicating
* whether the user is an operator or a regular user on this channel
*************************************************************/
typedef typename std::map<User *, std::string>	userDirectory;

class Channel
{
	public:

		Channel(std::string name, std::string key);
		~Channel();

		// setters
		void	setTopic(std::string name);
		int		handleModes(char mode, std::string params);
		void	setTargetMode(char c, std::string target);
		void	setUserLimit(std::string limit);
		void	setChannelMode(char c, std::string params);

		// getters
		std::string		getName(void) const;
		std::string		getTopic(void) const;
		userDirectory	getUserDirectory(void) const;
		std::string		getKey(void) const;
		size_t			getUserNbr(void) const;
		size_t			getUserLimit(void) const;
		std::string		*getUserMode(std::string name);

		// mode getters
		bool			isKeyProtected() const;
		bool			isTopicProtected() const;
		bool			isModerated() const;
		bool			isMembersOnly() const;
		bool			isSecret() const;
		bool			isPrivate() const;
		bool			isOper(std::string name);

		// commands
		void	join(User *user);
		void	part(User *user);
		void	names(void);
		bool	checkMode(char c) const;


	private:

		std::string		_name;
		std::string		_topic;
		std::string		_modes;
		std::string		_key;
		userDirectory	_users;
		size_t			_userLimit;
};

#endif
