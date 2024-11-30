#include "Server.hpp"

void Server::list(std::string buf, int fd)
{
	// std::cout << "--------------- LIST -----------------" << std::endl;
	std::string name = buf.substr(5);

	// if (hasOwnNick(_clients[index], nick))
	// 	return ;
	// if (isNickTaken(_clients, nick))
	// {
	// 	sendError("433: " + nick + ":Nickname is already in use", fd);
	// 	return ;
	// }
	// if (!isValidNick(nick))
	// {
	// 	sendError("432: " + nick + ":Erroneus nickname", fd);
	// 	return ;
	// }
	// std::cout << "Client " << fd << " set nickname to: " << nick << std::endl;
	// sendResponse(":" + _clients[index]->getNickname() + " NICK " + nick, fd);
	// _clients[index]->setNickname(nick);
}