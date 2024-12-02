#include "Server.hpp"

/*
	Registers and welcomes user to the server on success
	checks if user has authenticated and regirsted to the server
*/
void Server::user(std::string buf, int fd, int index)
{
	std::cout << "--------------- USER -----------------" << std::endl;

	// check if client exists, is authenticated and not already registerd
	/*
	make sure that reconnecting works,
	(letting the server run for 10 minutes) and isn't broken cause of this */
	if (!_clients[index] || !_clients[index]->getIsAuthenticated() || _clients[index]->getIsRegistered())
	{
		if (!_clients[index])
			std::cerr << "Error: Client " << fd << " not found." << std::endl;
		else if (!_clients[index]->getIsAuthenticated())
			sendError("491 : Password required to connect", fd);
		if (_clients[index]->getIsRegistered())
			std::cout << "Reregistration" << std::endl;
		else
			std::cout << "Authentication error" << std::endl;
		return;
	}

	// INVALID ERROR CODE;
	std::string user = buf.substr(5);
	if (user.empty() || _clients[index]->getNickname().empty())
	{
		if (user.empty())
			sendError("432: Invalid username", fd);
		else
			sendError("432 NICK: Nickname is required before registering to the server", fd);
		std::cout << "Nickname is required" << std::endl;
		return;
	}

	sendResponse(
		":localhost 001 " + _clients[index]->getNickname() + 
		" :- - - - - - - - - - - - - - - - - - - - - -\n"
		" ______   \n"
		"|      \\       (\\_/)\n"
		"|       \\     (o.o )  Welcome\n"
		"|  MAIL  |    ( :   \\  to our IRC\n"
		"|  BOX   |    (\\ /   )    server " + _clients[index]->getNickname() + "!\n"
		"|________|    \n"
		"- - - - - - - - - - - - - - - - - - - - -\n"
		"          \n"
		"Available commands:\n"
		"¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨\n"
		" -> \x02/nick\x02 <nickname> - Change your nickname\n"
		" -> \x02/msg\x02 <nickname> <message> - Send a private message\n"
		" -> \x02/join\x02 <#channel> - Join a channel\n"
		"    -> \x02/topic\x02 <#channel> <topic> - Set a topic for the channel\n"
		"    -> \x02/mode\x02 <#channel> <mode> (mode parameters) - Set a mode for the channel\n"
		"    -> \x02/kick\x02 <#channel> <nickname> (reason) - Kick a user from the channel\n"
		"    -> \x02/invite\x02 <nickname> <#channel> - Invite a user to the channel\n"
		" -> \x02/quit\x02 - Disconnect from the server\n"
		" -> \x02/list\x02 - Shows a list of channels\n"
	, fd);
	_clients[index]->setUsername(user);
	_clients[index]->setIsRegistered(true);
	std::cout << "Client " << user << " has been registered" << std::endl;
}
