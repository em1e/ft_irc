#include "Server.hpp"

void Server::user(std::string buf, int fd, int index)
{
	std::cout << "--------------- USER -----------------" << std::endl;
	if (_clients[index - 1]->getIsRegistered())
		return ;
	std::string user = buf.substr(5);
	if (_clients[index - 1]->getNickname().empty())
	{
		sendError("431 :No nickname given", fd);
		return ;
	}

	sendResponse(
		":localhost 001 :- - - - - - - - - - - - - - - - - -\n"
		"  _____   \n"
		" /     \\       (\\_/)\n"
		"/       \\     (o.o )  Welcome\n"
		"|  MAIL  |    ( :   \\  to our IRC\n"
		"|  BOX   |    (\\ /   )    server " + _clients[index - 1]->getNickname() + "!\n"
		"|________|    \n"
		" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	, fd);
	_clients[index - 1]->setUsername(user);
	_clients[index -1]->setIsRegistered(true);
}
