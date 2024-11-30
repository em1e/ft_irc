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
	// std::cout << "--------------- NICK -----------------" << std::endl;
	std::string nick = buf.substr(5);

	if (hasOwnNick(_clients[index], nick))
		return ;
	if (isNickTaken(_clients, nick))
	{
		sendError("433: " + nick + ":Nickname is already in use", fd);
		return ;
	}
	if (!isValidNick(nick))
	{
		sendError("432: " + nick + ":Erroneus nickname", fd);
		return ;
	}
	std::cout << "Client " << fd << " set nickname to: " << nick << std::endl;
	sendResponse(":" + _clients[index]->getNickname() + " NICK " + nick, fd);
	_clients[index]->setNickname(nick);
}
