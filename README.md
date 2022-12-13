# ircserv
This project is about creating our own IRC server in C++.<br />
Clients can register on the server and chat with each other.

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

Ex.:
nc -C localhost 6667
PASS pass
NICK myNick
User a b c d

// To use Irssi (our reference client)
irssi -p <port> -c <servername> -n <nickname> -w <password>
Ex.: irssi -p 6667 -c localhost -n myNick -w pass

// To connect from another computer at 42 school
irssi -p <port> -c <computer name> -n <nickname> -w <password>
Ex.: irssi -p 6667 -c e1r2p5 -n myNick -w pass

// Some useful Irssi commands
/reconnect => reconnect to server
/clear => clear tab
CTRL + N => toggle between tabs
```

* To set up and use DCC to transfer files between clients with Irssi
```
/* RECIPIENT CONFIGURATION */

// Accept low ports
/set dcc_autoaccept_lowports ON

// Automatically download files and received
/set dcc_autoget ON

// Set download path
/set dcc_download_path <path>
Ex.: /set dcc_download_path /mnt/nfs/homes/daisvke/Documents/ircserv/

// Allow file creation when copying the received file
/set dcc_file_create_mode 644

/* SENDER */

// Send file to the destination
/dcc send <recipient nick> <path to the file>
Ex.: /dcc send luc /mnt/nfs/homes/luc/Documents/file_to_send
```

* Handled commands:
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

* Handled modes:
```
// User modes
o

// Channel modes
timnslk

// Channel param modes
lk
```

## Screenshots
<p align="center">
  <img src="/screenshots/7-clients.png" width="80%" />
</p>
