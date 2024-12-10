## /topic <channel> <topic>:
- [x] change a topic of a channel when no restrictions, of a channel you're part and admin of
    - [x] when no topic is set
    - [x] when a topic was set previously (change topic)
- [ ] error cases:
    - [o] change topic when not admin
    - [ ] change topic when restrictions are on
    - [x] change topic of a channel that doesn't exist
    - [o] change topic to empty string
    - [ ] change topic when topic restrictions are on in the channel you're a part and admin of

## /list -YES:
- [x] shows all available channels, how many users (user count) and topic
    - [x] does it get updated when clients join channels
    - [x] does it get updated when channels are created
    - [ ] does it get updated when clients are kicked
    - [x] does it get updated topic is changed

## /kick <nickname> <channel>:
- [ ] kick client from a channel, that you're a part and an admin of
- [ ] error cases:
    - [ ] kick a non existant client from channel
    - [ ] kick client from a non existant channel
    - [ ] kick a non existant client from a non existant channel
    - [ ] kick client if not admin of the channel you're in
    - [ ] kick client (found from channel) that you're not a part of
    - [ ] kick yourself from a channel you're in
    - [ ] kick someone who isn't in the channel (client still exists)

## /invite <nickname> <channel>:
- [ ] invite client to invite only channel, that you're a part and an admin of
- [ ] error cases:
    - [ ] send invite to a non inviteOnly channel
    - [ ] send invite to channel with userlimit maxed out
    - [ ] send invite to a non existant client
    - [ ] send invite to a non existant channel
    - [ ] send invite to a non existant client and channel
    - [ ] send invite if not admin of the channel
    - [ ] send invite to a channel you're not a part of
    - [ ] send invite yourself
    - [ ] send invite to someone already inside the channel
    - [ ] send invite to existing channel when not authenticated
    - [ ] send invite to existing channel when not registed

## /mode <channel> <mode> (args):
- [ ] be able to set and remove userLimit
- [ ] be able to set and remove topicRestrictions
- [ ] be able to set and remove isPassword
- [ ] be able to set and remove inviteOnly
- [ ] be able to give and take admin perms
- [ ] error cases, userlimit:
    - [ ] set userLimit to be lower than user count in channel (how many users are in channel)
    - [ ] set / remove userlimit of a channel that doesn't exist
    - [ ] set / remove userlimit of a channel, when in channel but not admin
    - [ ] set / remove userlimit of a non existant channel
- [ ] error cases, admin perms:
    - [ ] give admin perms to client who's not in channel
    - [ ] give admin perms to client who changed their nickname when in channel
    - [ ] take admin perms away from client, make then run mode and topic commands
    - [ ] take admin perms away from yourself
    - [ ] take admin perms away from multiple users (should this be possible?)
    - [ ] give / take away multiple users admin perms (should this be possible?)
    - [ ] give / take away admin perms for a channel you're not a part or admin of
    - [ ] give / take away admin perms for a non existant channel
- [ ] general error cases:
    - [ ] run invalid mode commands
        - [ ] incorrect arg order
        - [ ] incorrect flags
        - [ ] too many args

## /join <channel>:
- [x] create and join a non existant channel, becoming the first admin
- [x] join existing channels
- [x] join channels you have been invited to (invite only)
- [o] join channels that require a password
- [o] join multiple channels at the same time
- [ ] error cases:
    - [ ] join channels that are past their userLimit
    - [ ] join channels that require a password, but incorrect password is given
    - [ ] join channels that require an invite (without one)
    - [ ] join channels you are already a part of
    - [ ] join channels when not registered
    - [ ] join channels when not authenticated
    - [o] join multiple channels, but some are invalid
    - [ ] join channels wrong password
    - [ ] join channels with password that don't have password enabled
    - [ ] join channels with password, but when not invited (inviteOnly is on)

## /msg <nickanme> <msg>:
- [x] message someone inside a channel
    - [x] can other clients in the channel see them // NO, a new privat window opens
    - [x] do error messages come through to sender
- [x] message someone outside a channel
- [x] check if you're outside a channel, and they are inside a channel (and error cases)
- [x] check if you're inside a channel, and they are outside a channel (and error cases)
- [x] error case:
    - [x] send non existant clients a message
    - [x] send an empty message
    - [x] send message when not authenticated
    - [x] send message when not registed
- [x] broadcast a message inside a channel
- [x] broadcast a message inside a channel (when in multiple channels)
- [x] broadcast a message when not in a channel (no /msg)
- [x] broadcast a message in window 1 when in a channel

## /nick <nickname>:
- [x] change inside a channel
    - [x] make sure users with their previous nickname can join and be made admin in the channel
    - [o] make sure the nickname change is broadcasted in all channels the client is in
    - [x] error cases:
        - [x] check if no nickname is given // checking current nickname
        - [x] check if someone already has that nickname // invalid and wont let through
        - [x] check for invalid nickname
            - [x] special characters, taken or other
- [x] change outside a channel
    - [x] error cases:
        - [x] check if no nickname is given // checking current nickname
        - [x] check if someone already has that nickname // invalid and wont let through
        - [x] check for invalid nickname
            - [x] special characters, taken or other

## Connection tests:
- [x] test connecting to server with ip address (not localhost)
    - [o] test error cases:
        - [o] invalid ip address
        - [o] invalid password
        - [o] no password
        - [x] include nickname in /connect
        - [x] invalid port
        - [x] no port
        - [o] try joining with nickname that already exists 
        - [x] include nickname in /connect and join server with another client (make sure nickname is not broken)

-------------------------------------------------------------------------------------

x = IT WORKS
o = SOMETHING IS WRONG, READ NOTES BELOW


## NOTES:
- mem issues:
  - using valgrind, why does the server close when two clients with the same nickname try joining? but not without it?
  - using valgrind, when disconencting all clients there are invalid reads
  - when joining a new channel, there are uninitialised values in mode
  - 

- Connection tests:
  - ~~connecting with an invalid ip address works on localhost for some reason~~
    - ~~e.g. 127.0.1.0 or 127.0.100.150~~
    - ~~127.0.1000.150 does not work (not valid)~~
  - ~~the index issue and user limit on invalid password hasn't been fixed~~

- nick:
  - for some reason if you join multiple channels at once, it doesn't sent the admin stuff or nickname changes into the later channels
  - need to announce that nickname has been changed in all priv and chan.

- privmsg:
  - if you open a chat with another client, change one of their nicknames then the msg then the open chat doesn't work anymore

- join:
  - ~~you can join channels using: #channel, channel, 4channel, _channel and .~~
    - ~~do we need channel name checks?~~
  - unable to join multiple channels at once
  ~~- if not inviited, the user trying to connect can send messages to channel, but not recieve messages sent to the channel~~
  - 

- topic:
  - on success, topic doesn't send a message to the channel admins? (the client who set the topic) for the change
        kimviljasimos changed the topic of #chan123hello to: hello (liberachat)
  - normal users can't change topic, _topicRestrictions needs to be checked
  - if topic string is empty, how does it act (check libaral chat)
  - changing topic with no param = sets topic to: No topic set
  - changing topic when not admin. doesnt let the user change topic
  - when new user enters. Topic for #chan123hello: hello should print (libera)
  - /topic -> should send what the topic is
  - if another user but admin changes it: <user> changed the topic of <channel> to: <topic>


- mode:
  - changing doesn't work, needs to toggle on or off _isChannelPassword
  - changing channel topic restrictions doesn't work, needs to toggle on or off _topicRestrictions
  - ServerMode/#chan123hello [+Cnst] by copper.libera.chat when creating a channel.
  - Cannot join to channel #chan123hello (Channel is full) if the channel is full. but it wont kick anybody which are already inside the channel. 
  - NEG num or 0. wont do anything. 