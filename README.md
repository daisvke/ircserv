# ircserv
This project is about creating our own IRC server.

## Commands
* ircserv
```
// Make and launch server with parameters

make && ./ircserv <port> <password>
Ex.: make && ./ircserv 6667 pass
```

* Clients
```
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

* To set up and use DCC to transfer files between clients
```
// Accept low ports
/set dcc_autoaccept_lowports ON

// Automatically download files and received
/set dcc_autoget ON

// Set download path
/set dcc_download_path <path>
Ex.: /set dcc_download_path /mnt/nfs/homes/daisvke/Documents/ircserv/

// Allow file creation when copying the received file
/set dcc_file_create_mode 644

// Send file to the destination
/dcc send
Ex.: /dcc send u1 /mnt/nfs/homes/daisvke/Documents/file_to_send

dcc send irssiLuc2 "/Documents/coucou.txt"

dcc get irssiLuc "coucou.txt"
```

Handled commands:<br />
```
PASS
NICK
USER
WHOIS
WHO
OPER
QUIT
JOIN
PART
MODE
TOPIC
NAME
LIST
INVITE
KICK
KILL
PING
PONG
```
