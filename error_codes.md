### Authentication and Registration
> - 462 (ERR_ALREADYREGISTERED): Attempt to register when already registered.
> - 464 (ERR_PASSWDMISMATCH): Incorrect password or authentication failure.

### User and Nickname Issues
> - 431 (ERR_NONICKNAMEGIVEN): No name provided.
> - 432 (ERR_ERRONEUSNICKNAME): Invalid or unusable name. 
> - 433 (ERR_NICKNAMEINUSE): The desired nickname is already in use by another client.

### Channel Issues
> - 403 (ERR_NOSUCHCHANNEL): Attempt to join or interact with a non-existent channel.
> - 442 (ERR_NOTONCHANNEL): Attempt to perform an action on a channel the user is not part of.
> - 461 (ERR_NEEDMOREPARAMS): If the channel name is missing.
> - 471 (ERR_CHANNELISFULL): Cannot join a channel because it is at capacity.
> - 473 (ERR_INVITEONLYCHAN): Attempt to join an invite-only channel without an invitation.
> - 482 (ERR_CHANOPRIVSNEEDED): If the client lacks operator privileges
> - 472 (ERR_UNKNOWNMODE): Unknown mode
> - 467 (ERR_KEYSET): Channel key already set
> - 475 (ERR_BADCHANNELKEY): Cannot join channel (incorrect key)
<!-- > - 477 (ERR_NOCHANMODES): Channel does not support modes -->
<!-- > - 324 (RPL_CHANNELMODEIS): <channel> <modes> <mode parameters> //Reply to a MODE command showing the current channel modes. -->
> - 331 (RPL_NOTOPIC): No topic is set
<!-- > - 332 (RPL_TOPIC): <channel> :<topic> //Reply to a TOPIC command showing the current channel topic -->

### General Errors
> - 401 (ERR_NOSUCHNICK): Target nickname does not exist or cannot be found.
> - 403 (ERR_NOSUCHCHANNEL): For a non-existent channel.
> - 402 (ERR_NOSUCHSERVER): Specified server does not exist or cannot be reached.
> - 404 (ERR_CANNOTSENDTOCHAN): Cannot send messages to a channel, often due to permissions.
> - 411 (ERR_NORECIPIENT): When no recipient is specified in the PRIVMSG command.
> - 412 (ERR_NOTEXTTOSEND) – No message text provided.
> - 421 (ERR_UNKNOWNCOMMAND): The command is not recognized by the server.
> - 481 (ERR_NOPRIVILEGES): Permission Denied- You're not an IRC operator

### Server-Specific
> - 421 (ERR_UNKNOWNCOMMAND): A command issued by the client is unrecognized by the server.
> - 451 (ERR_NOTREGISTERED): The client must register before issuing certain commands.
> - 441 (ERR_USERNOTINCHANNEL): They aren't on that channel
> - 491 (ERR_NOOPERHOST): No O-lines for your host //A client attempts an operator action without being properly authorized.
