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

## /join <channel>:
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
- [ ] change a topic of a channel when no restrictions, of a channel you're part and admin of
    - [ ] when no topic is set
    - [ ] when a topic was set previously (change topic)
- [ ] error cases:
    - [ ] change topic when not admin
    - [ ] change topic when restrictions are on
    - [ ] change topic of a channel that doesn't exist
    - [ ] change topic to empty string
    - [ ] change topic when topic restrictions are on in the channel you're a part and admin of

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