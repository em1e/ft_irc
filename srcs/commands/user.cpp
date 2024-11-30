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
			std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		else if (!_clients[index]->getIsAuthenticated())
			sendError("464 : Password required", fd);
		else
			sendError("462 : Cannot register again", fd);
		return;
	}

	std::string user = buf.substr(5);
	if (user.empty() || _clients[index]->getNickname().empty())
	{
		if (user.empty())
			sendError("432: Invalid username", fd);
		else
			sendError("432: Nickname is required before registering to the server", fd);
		return;
	}

	sendResponse(
		":localhost 001 :- - - - - - - - - - - - - - - - - - - - - -\n"
		"  _____   \n"
		" /     \\       (\\_/)\n"
		"/       \\     (o.o )  Welcome\n"
		"|  MAIL  |    ( :   \\  to our IRC\n"
		"|  BOX   |    (\\ /   )    server " + _clients[index]->getNickname() + "!\n"
		"|________|    \n"
		"- - - - - - - - - - - - - - - - - - - - -\n"
		"Please change your nickname to something else with /nick"
	, fd);
	_clients[index]->setUsername(user);
	_clients[index]->setIsRegistered(true);
}
