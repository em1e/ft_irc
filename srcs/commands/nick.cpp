#include "Server.hpp"

bool isNicknameTaken(const std::vector<Client *> &clients, const std::string &nick)
{
	for (const Client* client : clients)
	{
		if (client->getNickname() == nick)
			return true;
	}
	return false;
}

bool hasOwnNickname(Client* client, const std::string &nick)
{
	return client->getNickname() == nick;
}

void Server::nick(std::string buf, int fd, int index)
{
	std::cout << "--------------- NICK -----------------" << std::endl;
	std::string nick = buf.substr(5);

	if (hasOwnNickname(_clients[index -1], nick))
		return ;
	if (isNicknameTaken(_clients, nick))
	{
		sendError("433: " + nick + ":Nickname is already in use", fd);
		return;
	}

	std::cout << "Client " << fd << " set nickname to: " << nick << std::endl;
	sendResponse("Your nickname has been successfully set to: " + nick, fd);
	_clients[index - 1]->setNickname(nick);
}

// // 432     ERR_ERRONEUSNICKNAME
//                         "<nick> :Erroneus nickname"

//                 - Returned after receiving a NICK message which contains
//                   characters which do not fall in the defined set.  See
//                   section x.x.x for details on valid nicknames. /