# ft_irc
 develop a cool IRC server :)

## Talk about:
- when we'll meet together next, and in the future?
  -> schedules, holidays
- who does what and how we'll go from here
- bonuses, do we want to do them?
  - bot and handle file transfer
- what is our goal deadline? befire chriu

## Reserach:
 - ~~irc and how to start the server.~~
 - how the allowed functions work

## Create:
 - different getters and setters for all classes
 - server starting, signals, main fucntion, and fixup headers
 - create channels, channel management, permissions
 - add users and add those to servers
 - imprement commands

## Steps:
1- Begin by establishing a working client/server socket connection
2- Then build on that : add the signals handling (CTRL+C, CTRL+D, CTRL+Z);
3- Only then begin the client registration system (NICK, USER, PASS),
4- The server commands (PING, OPER, KILL...)
5- The channel operations commands (JOIN, PART, INVITE, KICK, PRIVMSG, NOTICE...).

## Commands:
Client - 
> NICK, USER, PASS
Channel commands - 
> JOIN, PART, TOPIC, INVITE, KICK, QUIT, MODE, and PRIVMSG


## Features
- Multi-threaded architecture for handling concurrent - client connections.
- Support for multiple simultaneous connections.
- Creation and management of IRC channels.
- User authentication and registration.
- Broadcasting messages to all users in a channel.
- Private messaging between users.
- Handling of various IRC commands such as JOIN, PRIVMSG, PART, etc.
- Support for user nicknames and channel names.
- Connect to the IRC server.
- Join channels and participate in group conversations.
- Send and receive messages.
- Change user nickname.
- Send private messages to other users

## Notes:
> A socket is an endpoint that enables two processes to communicate with each other, either on the same machine or across a network. It acts as an interface between the application layer and the transport layer of the networking stack.

> use dynamic macros for automated / formated messages
>
> e.g. `# define RPL_JOIN(user_id, channel) (user_id + " JOIN :#" +  channel + "\r\n")`

