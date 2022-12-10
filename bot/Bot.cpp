/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 14:15:27 by lchan             #+#    #+#             */
/*   Updated: 2022/12/10 18:58:58 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "BotUtils.hpp"

/*********************************************
 * 				constructor / destructor
 *********************************************/
Bot::Bot(int port, std::string pwd): _port(port), _pwd(pwd), _chanName(CHAN_NAME),
									 _status(OFF_STATUS), _flag(0), _rplIndex(0) {setupBot( );}

Bot::~Bot() { if (_listenBSd) close (_listenBSd);}

/*********************************************
 * 				setup bot
 *********************************************/
void Bot::setBotMap()
{
	_botMap[0] = "You are welcome";
	_botMap[1] = "I was created at " + ircTime();
	_botMap[2] = "Hello, new member, how are you ?";
	_botMap[3] = "There's more to learn!";
	_botMap[4] = "Let me teach you the ways of magic!";
	_botMap[5] = "I got quests!";
	_botMap[6] = "Magic waits for no one, apprentice!";
	_botMap[7] = "Still working on that quest?";
	_botMap[8] = "Shouldn't you be murdering something about now?";
	_botMap[9] = "No way! That's, like, my third favorite kind of magic!";
	_botMap[10] = "Hey! You're TALKING to me! And I didn't even have an exclamation point over my head! This is the BEST day of my life!";
	_botMap[11] = "Sooooo... how are things?";
	_botMap[12] = "Hey, best friend!";
	_botMap[13] = "Yessss, look into my eyes. You're getting sleepy. You're getting... zzzzzz... Zzzzzz...";
	_botMap[14] = "Success! My spell to make you want to hang out with me worked!";
	_botMap[15] = "Stay a while, and listen. Oh god, please -- PLEASE! -- stay a while.";
	_botMap[16] = "Away with thee!";
	_botMap[17] = "Hocus pocus!";
	_botMap[18] = "Ahhh!";
	_botMap[19] = "Alaka-ZAM!";
	_botMap[20] = "Ha-HA!";
}

void	Bot::setupBot()
{
	ircMemset((void *)_buffer, 0, sizeof(_buffer));
	setBotMap();
	try{
		connectToServer();
		authentification();
		creatChannel();
		checkServerRpl();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
		return ;
	}
	botExec();
}

void	Bot::connectToServer()
{
	int opt = 1;

	if ((_listenBSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == BOT_ERROR)
		throw std::invalid_argument("Bot socket fail");
 	if (setsockopt(_listenBSd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		throw std::invalid_argument("Bot setsockopt fail");
	_addrBot.sin_family = AF_INET;
	_addrBot.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	_addrBot.sin_port = htons(_port);
	if (connect(_listenBSd, (sockaddr *) &_addrBot, sizeof(_addrBot)) == BOT_ERROR)
		throw std::invalid_argument("bot connect fail");
	std::cout << "Bot is now connected" << std::endl;
	_status = ON_STATUS;
}

void	Bot::authentification()
{
	std::string msg =	std::string(BOT_CONN_CAP)
						+ std::string(BOT_CONN_PASS(std::string(_pwd)))
						+ std::string(BOT_CONN_NICK)
						+ std::string(BOT_CONN_USER);
	// TODO :: add authentification as operator;
 	send(_listenBSd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void	Bot::creatChannel()
{
	std::string msg =	std::string(BOT_JOIN_CHAN(_chanName, " botpass\r\n"));
	send(_listenBSd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void	Bot::checkServerRpl()
{
	std::string rplTab[7] = {"JOIN", "001", "002", "003", "004", "353", "366"};

	do {
		waitForCompleteRecv();
		if (_strBuffer.find("You're now known as Bot") != std::string::npos){
			extractCmd(_strBuffer);
			continue ;
		}
		_splitBuffer = ircSplit(extractCmd(_strBuffer), ' ');
		for (int i = 0; i < 7; ++i){
			if (_splitBuffer.size() < 1)
				throw std::invalid_argument("Channel creation failed");
			else if (rplTab[i] == _splitBuffer[1])
				_flag |= (1<<i);
		}
		if (_splitBuffer[1] == "475")
			throw std::invalid_argument("Channel seems pwd protected");
	} while (_flag < 63);
	if (_flag > 63)
		throw std::invalid_argument("unexpected error");
	std::cout << "Bot authentified \n" << std::endl;
}

/*********************************************
 * 		make reply
 *********************************************/

void	Bot::setRplBuffer()
{
	//_rplBuffer = "PRIVMSG #BotChan :";
	_rplBuffer = "PRIVMSG " + std::string(CHAN_NAME) + " :";
	_rplBuffer += _botMap[_rplIndex++];
	if (_rplIndex > 20)
		_rplIndex = 0;
}

void	Bot::sendRpl()
{
	std::string	str;
	int			sendRet = 0;

	while (1){
		str = extractCmd(_strBuffer);
		if (str.empty())
			break;
		setRplBuffer();
		if (_rplBuffer.empty() == false){
			_rplBuffer += "\r\n";
			sendRet = send(_listenBSd, _rplBuffer.c_str(), _rplBuffer.size(), MSG_NOSIGNAL);
			if (sendRet < 0)
			{
				std::cout << "bot error send() failed" << std::endl;
				_status = OFF_STATUS;
			}
			_rplBuffer.clear();
		}
	}
}

void	Bot::readBuffer()
{
	int	recvRet;

	recvRet = recv(_listenBSd, _buffer, sizeof(_buffer), 0);
	if (recvRet < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cout << "recv() failed" << std::endl;
			_status = OFF_STATUS;
			return ;
		}
	}
	else if (recvRet == 0)
		_status = OFF_STATUS;
}

void	Bot::concatRecv()
{
	_strBuffer += _buffer;
	ircMemset((void *)_buffer, 0, sizeof(_buffer));
}

void	Bot::waitForCompleteRecv()
{
	while ((_strBuffer.empty() || _strBuffer.find("\r\n") == std::string::npos)
		&& _status == ON_STATUS)
	{
		readBuffer();
		concatRecv();
	}
	std::cout << "received = " << _strBuffer << std::endl;
}

void	Bot::botExec()
{
	while (_status == ON_STATUS)
	{
		waitForCompleteRecv();
		if (_strBuffer.find("403 " + std::string(BOTNAME)) != std::string::npos)
			_status = OFF_STATUS;
		else
			sendRpl();
	}
}
