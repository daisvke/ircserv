# **ircserv**

## **Description**  
This project involves creating a custom **IRC (Internet Relay Chat) server** in C++. The server allows clients to register, connect, and chat with each other, adhering to the IRC protocol.

---

## **Commands**

### **Launching the Server**  
Compile and launch the server with the following command:  
```bash
make && ./ircserv <port> <password>
# Example:
make && ./ircserv 6667 pass
```

---

### **Connecting Clients**  
#### **Using `nc` (Netcat) as a Client**  
Connect to the server using the `nc` utility:  
```bash
nc -C <servername> <port>
PASS <password>
NICK <nickname>
USER <username> <hostname> <servername> <realname>
# Example:
nc -C localhost 6667
PASS pass
NICK myNick
USER user host server RealName
```

#### **Using `irssi` (Reference Client)**  
Connect using the `irssi` client:  
```bash
irssi -p <port> -c <servername> -n <nickname> -w <password>
# Example:
irssi -p 6667 -c localhost -n myNick -w pass
```

#### **Connecting from Another Computer (e.g., 42 School)**  
```bash
irssi -p <port> -c <computer_name> -n <nickname> -w <password>
# Example:
irssi -p 6667 -c e1r2p5 -n myNick -w pass
```

---

### **Useful `irssi` Commands**  
- `/reconnect`: Reconnect to the server.  
- `/clear`: Clear the current tab.  
- **`CTRL + N`**: Toggle between tabs.

---

### **Setting Up and Using DCC (Direct Client-to-Client) File Transfers**  
#### **Recipient Configuration**  
1. **Accept low ports:**  
   ```bash
   /set dcc_autoaccept_lowports ON
   ```
2. **Automatically accept and download received files:**  
   ```bash
   /set dcc_autoget ON
   ```
3. **Set download path:**  
   ```bash
   /set dcc_download_path <path>
   # Example:
   /set dcc_download_path /mnt/nfs/homes/daisvke/Documents/ircserv/
   ```
4. **Allow file creation during downloads:**  
   ```bash
   /set dcc_file_create_mode 644
   ```

#### **Sender Configuration**  
Send a file to a recipient:  
```bash
/dcc send <recipient_nick> <file_path>
# Example:
/dcc send luc /mnt/nfs/homes/luc/Documents/file_to_send
```

---

## **Handled Commands**  
The server supports the following IRC commands:  
```
PASS   NICK   USER   WHOIS   WHO  
OPER   QUIT   JOIN   PART    MODE  
TOPIC  NAMES  LIST   INVITE  KICK  
KILL   PING   PONG
```

---

## **Handled Modes**  

### **User Modes:**  
| **Mode** | **Description**        |  
|----------|------------------------|  
| `o`      | Operator mode          |  

### **Channel Modes:**  
| **Mode** | **Description**                     |  
|----------|-------------------------------------|  
| `t`      | Topic protection                    |  
| `i`      | Invite-only channel                 |  
| `m`      | Moderated channel                   |  
| `n`      | No external messages                |  
| `s`      | Secret channel                      |  
| `l`      | User limit                          |  
| `k`      | Channel key (password)              |  

### **Channel Parameter Modes:**  
| **Mode** | **Description**                     |  
|----------|-------------------------------------|  
| `l`      | Limit the number of users           |  
| `k`      | Set a password for the channel      |  

---

## **Screenshots**  
<p align="center">
  <img src="/screenshots/7-clients.png" />
  A scene from <i>Lady Windermere's Fan</i>, by Oscar Wilde.
</p>

---

## **Additional Resources**  
1. **Modern IRC Documentation:**  
   [https://modern.ircdocs.horse/](https://modern.ircdocs.horse/)  
2. **IRC Protocol Examples:**  
   [http://chi.cs.uchicago.edu/chirc/irc_examples.html](http://chi.cs.uchicago.edu/chirc/irc_examples.html)
