#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "main.hpp"
#include "User.hpp"

enum e_chanReturn
{
	SUCCESS,
	ERROR
};


/*************************************************************
 * A map including a pointer to the users and indicating
 * whether the user is an operator or a regular user on this channel
 *************************************************************/
typedef std::map<User *, std::string> userDirectory;

class Channel
{
public:
	Channel(std::string name, std::string key);
	~Channel();

	// setters
	void setTopic(std::string name);
	bool modifyModes(char mode, std::string params, char sign);
	void modifyUserMode(char c, std::string target, char sign);
	void setUserLimit(std::string limit);
	bool setChannelMode(char c, std::string params);

	// getters
	std::string getName(void) const;
	std::string getTopic(void) const;
	std::string getModes(void) const;
	userDirectory *getUserDirectory(void);
	std::string getKey(void) const;
	size_t countUsers(void) const;
	size_t getUserLimit(void) const;
	std::string *getUserMode(std::string name);

	// mode getters
	bool isKeyProtected(void) const;
	bool isTopicProtected(void) const;
	bool isModerated(void) const;
	bool isInviteOnly(void) const;
	bool isSecret(void) const;
	bool isInternalOnly(void) const;
	bool isLimited(void) const;
	bool isOper(std::string name);
	bool hasVoice(std::string name);
	bool isEmpty(void) const;
	bool isMember(std::string nick);

	// commands
	void join(User *user, bool isOp);
	void part(User *user);
	void names(void);
	bool checkMode(char c) const;

private:
	std::string _name;
	std::string _topic;
	std::string _modes;
	std::string _key;
	userDirectory _users;
	size_t _userLimit;
};

#endif
