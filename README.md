# ircserv
This project is about creating our own IRC server.

## Commands
```
// Make and launch server with parameters

make && ./ircserv <port> <password>
Ex.: make && ./ircserv 6667 pass

// To use nc as a client

nc -C <servername> <port>
PASS <password>
NICK <nickname>
USER <username> <hostname> <servername> <realname>

Ex.: nc -C localhost 6667
PASS pass
NICK myNick
User a b c d

// To use Irssi (our reference client)
irssi -p <port> -c <servername> -n <nickname> -w <password>
Ex.: irssi -p 6667 -c localhost -n myNick -w pass
```


Handled commands:<br />
```
PASS
NICK
USER
WHOI
WHO
OPER
QUIT
JOIN
PART
MODE
TOPI
NAME
LIST
INVI
KICK
KILL
PING
PONG
```
