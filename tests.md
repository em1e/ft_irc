## /nick <nickname>:
- [ ] change inside a channel
    - [ ] make sure users with their previous nickname can join and be made admin in the channel
    - [ ] make sure the nickname change is broadcasted in all channels the client is in
- [ ] change outside a channel
- [ ] error cases:
    - [ ] check if no nickname is given
    - [ ] check if someone already has that nickname
    - [ ] check for invalid nickname
        - [ ] special characters, taken or other
    - [ ] check if client is authenticated

## /msg <nickanme> <msg>:
- [ ] message someone inside a channel
    - [ ] can other clients in the channel see them
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
- [ ] broadcast a message 

## /invite <nickname> <channel>:
- [x] invite client to invite only channel, that you're a part and an admin of
- [ ] error cases:
    - [x] send invite to a non inviteOnly channel **// gives error s it's fine**
    - [x] send invite to channel with userlimit maxed out **// it is still inviting the extra user**
    - [x] send invite to a non existant client **// gives error so it's fine**
    - [x] send invite to a non existant channel **// gives error**
    - [x] send invite to a non existant client and channel **// gives error so it's fine**
    - [x] send invite if not admin of the channel
    - [x] send invite to a channel you're not a part of
    - [x] send invite yourself **// it allows us to do it**
    - [x] send invite to someone already inside the channel **// it allows to do it**
    - [ ] send invite to existing channel when not authenticated
    - [ ] send invite to existing channel when not registed

## /join <channel>:
- [x] create and join a non existant channel, becoming the first admin
- [x] join existing channels
- [ ] join channels you have been invited to (invite only)
- [ ] join channels that require a password
- [ ] join multiple channels at the same time
- [ ] error cases:
    - [ ] join channels that are past their userLimit
    - [ ] join channels that require a password, but incorrect password is given
    - [x] join channels that require an invite (without one) **// gives an error**
    - [x] join channels you are already a part of /**/ does not do anything**
    - [ ] join channels when not registered
    - [ ] join channels when not authenticated
    - [ ] join multiple channels, but some are invalid
    - [ ] join channels wrong password
    - [ ] join channels with password that don't have password enabled
    - [ ] join channels with password, but when not invited (inviteOnly is on)

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

## /topic <channel> <topic>:
- [x] change a topic of a channel when no restrictions, of a channel you're part and admin of
    - [x] when no topic is set
    - [x] when a topic was set previously (change topic) 
- [x] error cases:
    - [x] change topic when not admin
    - [x] change topic when restrictions are on
    - [x] change topic of a channel that doesn't exist
    - [x] change topic to empty string
    - [x] change topic when topic restrictions are on in the channel you're a part and admin of

## /mode <channel> <mode> (args):
- [x] be able to set and remove userLimit
- [x] be able to set and remove topicRestrictions
- [x] be able to set and remove isPassword
- [x] be able to set and remove inviteOnly
- [x] be able to give and take admin perms **// does not work**
- [ ] error cases, userlimit:
  - [x] set userLimit to be lower than user count in channel (how many users are in channel) **// it allows us t change the number to anything >= 0**
  - [x] set / remove userlimit of a channel that doesn't exist
  - [x] set / remove userlimit of a channel, when in channel but not admin
  - [ ] set / remove userlimit of a non existant channel
- [ ] error cases, admin perms:
  - [x] give admin perms to client who's not in channel
  - [ ] give admin perms to client who changed their nickname when in channel
  - [ ] take admin perms away from client, make then run mode and topic commands
  - [ ] take admin perms away from yourself
  - [ ] take admin perms away from multiple users (should this be possible?)
  - [ ] give / take away multiple users admin perms (should this be possible?)
  - [ ] give / take away admin perms for a channel you're not a part or admin of
  - [ ] give / take away admin perms for a non existant channel
- [ ] general error cases:
  - [x] run invalid mode commands
    - [ ] incorrect arg order
    - [ ] incorrect flags
    - [ ] too many args

## Connection tests:
- [ ] test connecting to server with ip address (not localhost)
- [ ] test connecting to server with users from different computers
- [ ] test error cases:
    - [ ] invalid ip address
    - [ ] invalid password
    - [ ] no password
    - [ ] include nickname in /connect
    - [ ] invalid port
    - [ ] no port
    - [ ] try joining with nickname that already exists 
    - [ ] include nickname in /connect and join server with another client (make sure nickname is not broken)

## /list -YES:
- [ ] shows all available channels, how many users (user count) and topic
    - [ ] does it get updated when clients join channels
    - [ ] does it get updated when channels are created
    - [ ] does it get updated when clients are kicked
    - [ ] does it get updated topic is changed
- [ ] error cases:
    - [ ] invalid arg
        - [ ] "-YESSS", "-NO", "HAHA"
    - [ ] no args
    - [ ] too many args

