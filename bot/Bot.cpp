/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchan <lchan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 14:15:27 by lchan             #+#    #+#             */
/*   Updated: 2022/12/08 23:54:39 by lchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "BotUtils.hpp"

/*********************************************
 * 				constructor / destructor
 *********************************************/

Bot::Bot(int port, std::string pwd): _port(port), _pwd(pwd), _chanName("BotChan"),
									 _status(OFF_STATUS), _flag(0)
{setupBot( );
}

Bot::~Bot() { if (_listenBSd) close (_listenBSd); std::cout << "bot destroyed" << std::endl; }

/*********************************************
 * 				setup bot
 *********************************************/
void Bot::setBotMap()
{
	_botMap["hello"] = "Hello, new member, how are you ?";
	_botMap["Hello"] = "Hello, new member, how are you ?";
	_botMap["HELLO"] = "Hello, new member, how are you ?";
	_botMap["Time"] = "it is : " + ircTime();
	_botMap["Thank"] = "you are welcome";
}

void	Bot::setupBot(){

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

void	Bot::connectToServer(){

	int opt = 1;

	if ((_listenBSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == BOT_ERROR)
		throw std::invalid_argument("Bot socket fail");
	else
		std::cout << " Bot socket success" << std::endl;
 	if (setsockopt(_listenBSd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		throw std::invalid_argument("Bot setsockopt fail");
	else
		std::cout << " Bot socketopt success" << std::endl;
	_addrBot.sin_family = AF_INET;
	_addrBot.sin_addr.s_addr = inet_addr(LOCAL_HOST);
	_addrBot.sin_port = htons(_port);
	if (connect(_listenBSd, (sockaddr *) &_addrBot, sizeof(_addrBot)) == BOT_ERROR)
		throw std::invalid_argument("bot connect fail");
	else
		std::cout << "bot is now connected" << std::endl;

	_status = ON_STATUS;
}

void	Bot::authentification()
{
	std::string msg =	std::string(BOT_CONN_CAP)
						+ std::string(BOT_CONN_PASS(std::string(_pwd)))
						+ std::string(BOT_CONN_NICK)
						+ std::string(BOT_CONN_USER);

	// TODO :: add authentification as operator;
 	send(_listenBSd, msg.c_str(), msg.size(), 0);
}

void	Bot::creatChannel()
{
	std::string msg =	std::string(BOT_JOIN_CHAN(std::string(_chanName)));
	send(_listenBSd, msg.c_str(), msg.size(), 0);
}

void	Bot::checkServerRpl()
{
	int	i = 0;
	int cnt = 0;
	std::string rplTab[6] = {"001", "002", "003", "004", "353", "366"};

	std::cout << " initial _strBuffer " << _strBuffer << std::endl;
	do {
		std::cout << ">>>>>>>>>>> loop nbr : " << cnt++ << std::endl;
		waitForCompleteRecv();
		std::cout << "complete Recv = " << _strBuffer << std::endl;
		if (_strBuffer.find("You're now known as Bot"))
		{
			extractCmd(_strBuffer);
			std::cout << "after extraction" << _strBuffer << std::endl;
			continue ;
		}
		// else if (_strBuff.find(" "));
		// 	throw std::invalid_argument("Channel creation failed");
		_splitBuffer = ircSplit(extractCmd(_strBuffer), ' ');
		for (unsigned long j = 0; j < _splitBuffer.size(); ++j)
			std::cout << j  << " : "<< _splitBuffer[j] << std::endl;
		for (; i < 6; ++i){
			if (_splitBuffer.size() < 1)
				throw std::invalid_argument("Channel creation failed");
			else if (rplTab[i] == _splitBuffer[1])
			{
				std::cout << _flag << std::endl;
				_flag += ircStoi(rplTab[i]);
			}
		}
	} while (_flag < 729);

	if (_flag > 729)
	{
		std::cout << "_flag = " << _flag << std::endl;
		throw std::invalid_argument("unexpected error");
	}
	else
		std::cout << "bot is lauched \n" << std::endl;
}

/*********************************************
 * 		make reply
 *********************************************/

void	Bot::setRplBuffer(){

		botMap::iterator it = _botMap.find(_splitBuffer[0]);;
		if (it != _botMap.end())
			_rplBuffer = _botMap[_splitBuffer[0]];
		else
			_rplBuffer = "I dont get it";
}

void	Bot::sendRpl()
{
	std::string	str;

	while (1){
		str = extractCmd(_strBuffer);
		if (str.empty())
			break;
		std::cout << "extracted = " << str << std::endl;
		setRplBuffer();
		if (_rplBuffer.empty() == false){
			std::cout << "sending : " << _rplBuffer << std::endl;
			send(_listenBSd, _rplBuffer.c_str(), _rplBuffer.size(), 0);
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
	else if (recvRet == 0) // server turned of
		_status = OFF_STATUS;
}

void	Bot::concatRecv()
{
	_strBuffer += _buffer;
	ircMemset((void *)_buffer, 0, sizeof(_buffer));
	//std::cout << "client _strBuffer contains : " << _strBuffer << std::endl;
}

void	Bot::waitForCompleteRecv()
{
	do {

		readBuffer();
		concatRecv();
	} while(_strBuffer.find("\r\n") == std::string::npos);
}

void	Bot::botExec()
{
	while (_status == ON_STATUS)
	{
		waitForCompleteRecv();
		sendRpl();
	}
}

/*
client _buffer contains : [:localhost 001 Bot :Welcome to the IRC-LCHAN-DTANIGAWcom Network, BonusBot
]
client _buffer contains : [:localhost 002 Bot :Your host is localhost, running version 0.0.1
]
client _buffer contains : [:localhost 003 Bot :This server was created Thu Dec  8 17:54:09 2022
]
client _buffer contains : [:localhost 004 Bot localhost 0.0.1 ovinsk ovtimnsplk ovlk
]
client _buffer contains : [:localhost 353 Bot = BotChan :@Bot
]
client _buffer contains : [:localhost 366 Bot BotChan
*/
