#include "Server.hpp"

bool isValidNick(const std::string &nick)
{
	if (nick.empty())
		return false;
	if (!std::isalpha(nick[0]))
		return false;
	for (char c : nick)
	{
		if (!std::isalnum(c) && c != '-' && c != '_' && c != '~' && c != '.')
		return false;
	}
	return true;
}

bool isNickTaken(const std::vector<Client *> &clients, const std::string &nick)
{
	for (const Client* client : clients)
	{
		if (client->getNickname() == nick)
			return true;
	}
	return false;
}

bool hasOwnNick(Client* client, const std::string &nick)
{
	return client->getNickname() == nick;
}

void Server::nick(std::string buf, int fd, int index)
{
	std::cout << "--------------- NICK -----------------" << std::endl;
	std::string nick = buf.substr(5);

	// check if client exists and is authenticated
	if (!_clients[index] || !_clients[index]->getIsAuthenticated())
	{
		if (!_clients[index])
			std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		else
			sendError("464: Password required", fd);
		return;
	}

	// checks that nickname being set is not the same as what they already have
	if (hasOwnNick(_clients[index], nick))
		return ;

	if (isNickTaken(_clients, nick))
	{
		sendError("433: " + _clients[index]->getNickname() + nick + ":Nickname is already in use", fd);
		return ;
	}

	// check nickname formatting
	if (!isValidNick(nick))
	{
		sendError("432: " + nick + ":Erroneus nickname", fd);
		return ;
	}

	std::cout << "Client " << fd << " set nickname to: " << nick << std::endl;
	sendResponse(":" + _clients[index]->getNickname() + " NICK " + nick, fd);

	// broadcast name change to all other clients?

	_clients[index]->setNickname(nick);
}
