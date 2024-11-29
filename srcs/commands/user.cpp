#include "Server.hpp"

void Server::user(std::string buf, int fd, int index)
{
	(void)buf;
	// put username into username and make sure it doesn't alr exist
	std::cout << "--------------- USER -----------------" << std::endl;
	std::cout << "Client " << fd << " sent USER command." << std::endl;
	if (_clients[index - 1]->getNickname().empty())
	{
		std::cout << "user doesn't have a nickname, unable to welcome them!" << std::endl;
		sendResponse(":localhost 001 Error: you need to set a nickname first!", fd);
		return ;
	}

	sendResponse(
		"- - - - - - - - - - - - - - - - - -\n"
		"  _____   \n"
		" /     \\       (\\_/)\n"
		"/       \\     (o.o )  Welcome\n"
		"|  MAIL  |    ( :   \\  to our IRC\n"
		"|  BOX   |    (\\ /   )    server " + _clients[index - 1]->getNickname() + "!\n"
		"|________|    \n"
		" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	, fd);

	// _clients[index - 1]->setUsername(USERNAME HERE);
}