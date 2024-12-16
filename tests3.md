## /topic <channel> <topic>: // simos
- [ ] change a topic of a channel when no restrictions, of a channel you're part and admin of
    - [ ] when no topic is set
    - [ ] when a topic was set previously (change topic)
- [ ] error cases:
    - [ ] change topic when not admin
    - [ ] change topic when restrictions are on
    - [ ] change topic of a channel that doesn't exist
    - [ ] change topic to empty string
    - [ ] change topic when topic restrictions are on in the channel you're a part and admin of

## /list -YES: // kim
- [ ] shows all available channels, how many users (user count) and topic
    - [ ] does it get updated when clients join channels
    - [ ] does it get updated when channels are created
    - [ ] does it get updated when clients are kicked
    - [ ] does it get updated topic is changed

## /kick <nickname> <channel>: // vilja
- [ ] kick client from a channel, that you're a part and an admin of
- [ ] error cases:
    - [ ] kick a non existant client from channel
    - [ ] kick client from a non existant channel
    - [ ] kick a non existant client from a non existant channel
    - [ ] kick client if not admin of the channel you're in
    - [ ] kick client (found from channel) that you're not a part of
    - [ ] kick yourself from a channel you're in
    - [ ] kick someone who isn't in the channel (client still exists)

## /invite <nickname> <channel>: // vilja
- [ ] invite client to invite only channel, that you're a part and an admin of
- [ ] error cases:
    - [ ] send invite to a non inviteOnly channel
    - [ ] send invite to channel with userlimit maxed out
    - [ ] send invite to a non existant client
    - [ ] send invite to a non existant channel
    - [ ] send invite to a non existant client and channel **gives error message only for client**
    - [ ] send invite if not admin of the channel
    - [ ] send invite to a channel you're not a part of
    - [ ] send invite yourself
    - [ ] send invite to someone already inside the channel
    - [ ] send invite to existing channel when not authenticated **gives error that no such channel exists**
    - [ ] send invite to existing channel when not registed

## /mode <channel> <mode> (args): // simos
- [ ] be able to set and remove userLimit
- [ ] be able to set and remove topicRestrictions
- [ ] be able to set and remove isPassword
- [ ] be able to set and remove inviteOnly
- [ ] error cases, userlimit:
    - [ ] set userLimit to be lower than user count in channel (how many users are in channel)
    - [ ] set / remove userlimit of a channel that doesn't exist
    - [ ] set / remove userlimit of a channel, when in channel but not admin
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

## /join <channel>: // vilja
- [ ] create and join a non existant channel, becoming the first admin
- [ ] join existing channels
- [ ] join channels you have been invited to (invite only)
- [ ] join channels that require a password
- [ ] join multiple channels at the same time
- [ ] error cases:
    - [ ] join channels that are past their userLimit
    - [ ] join channels that require a password, but incorrect password is given
    - [ ] join channels that require an invite (without one)
    - [ ] join channels you are already a part of
    - [ ] join channels when not registered
    - [ ] join channels when not authenticated
    - [ ] join multiple channels, but some are invalid
    - [ ] join channels wrong password
    - [ ] join channels with password that don't have password enabled **gives wrong error message**
    - [ ] join channels with password, but when not invited (inviteOnly is on)

## /msg <nickanme> <msg>: // kim
- [ ] message someone inside a channel
    - [ ] can other clients in the channel see them // NO, a new privat window opens
    - [ ] do error messages come through to sender
- [ ] message someone outside a channel
- [ ] check if you're outside a channel, and they are inside a channel (and error cases)
- [ ] check if you're inside a channel, and they are outside a channel (and error cases)
- [ ] error case:
    - [ ] send non existant clients a message
    - [ ] send an empty message
    - [ ] send message when not authenticated
    - [ ] send message when not registed
- [ ] broadcast a message inside a channel
- [ ] broadcast a message inside a channel (when in multiple channels)
- [ ] broadcast a message when not in a channel (no /msg)
- [ ] broadcast a message in window 1 when in a channel

## /nick <nickname>: // kim
- [ ] change inside a channel
    - [ ] make sure users with their previous nickname can join and be made admin in the channel
    - [ ] make sure the nickname change is broadcasted in all channels the client is in
    - [ ] error cases:
        - [ ] check if no nickname is given // checking current nickname
        - [ ] check if someone already has that nickname // invalid and wont let through
        - [ ] check for invalid nickname
            - [ ] special characters, taken or other
- [ ] change outside a channel
    - [ ] error cases:
        - [ ] check if no nickname is given // checking current nickname
        - [ ] check if someone already has that nickname // invalid and wont let through
        - [ ] check for invalid nickname
            - [ ] special characters, taken or other

## Connection tests: // simos
- [ ] test connecting to server with ip address (not localhost)
    - [ ] test error cases:
        - [ ] invalid ip address
        - [ ] invalid password
        - [ ] no password
        - [ ] include nickname in /connect
        - [ ] invalid port
        - [ ] no port
        - [ ] try joining with nickname that already exists 
        - [ ] include nickname in /connect and join server with another client (make sure nickname is not broken)

-------------------------------------------------------------------------------------

x = IT WORKS
o = SOMETHING IS WRONG, READ NOTES BELOW


## NOTES:
- mem issues:
  - ~~using valgrind, why does the server close when two clients with the same nickname try joining? but not without it?~~
  - ~~using valgrind, when disconencting all clients there are invalid reads~~
  - ~~when joining a new channel, there are uninitialised values in mode~~

- Connection tests:
  - ~~connecting with an invalid ip address works on localhost for some reason~~
    - ~~e.g. 127.0.1.0 or 127.0.100.150~~
    - ~~127.0.1000.150 does not work (not valid)~~
  - ~~the index issue and user limit on invalid password hasn't been fixed~~

- nick:

- join:
  - ~~you can join channels using: #channel, channel, 4channel, _channel and .~~
    - ~~do we need channel name checks?~~
  ~~- if not inviited, the user trying to connect can send messages to channel, but not recieve messages sent to the channel~~
  - When the admin sets the channel to be invite Only and invites the other client and then the client tries to join the channel a windows opens that is blank..
  - when the channel is password protected and has a password,  the client can join without putting a password
  - When the client tries to join to different channels, we get a error message "Invalid chsnnel name" 
  - When userLimit = 2 and a 3rd client tries to join the channel, server gives an error that "cannot add client to the chsnnel" but then we get a broadcasting message that the client joins the channel and then we see that in the client side the client indeed joins the channel
  - Cannot join to channel #chan123hello (Channel is full) if the channel is full. but it wont kick anybody which are already inside the channel. 


- mode:
  - ~~changing doesn't work, needs to toggle on or off _isChannelPassword~~
  - ~~changing channel topic restrictions doesn't work, needs to toggle on or off _topicRestrictions~~
  ~~- client sends: MODE nickname +i automatically on join, handle that and don't give a channel error~~

	- we have some memory issues when last client disconnects from the server (using fsanitize)