#include "Server.hpp"

bool isValidNick(const std::string &nick)
{
	if (nick.empty())
		return false;
	if (nick.length() > 9)
		return false;
	if (!std::isalpha(nick[0]))
		return false;
	for (char c : nick)
	{
		if (!std::isalnum(c) && c != '-' && c != '~' && c != '.') //&& c != '_') // this fixes the same nick join mem issues
		return false;
	}
	return true;
}

bool isNickTaken(const std::vector<std::shared_ptr<Client>>& clients, const std::string& nick)
{
	for (const std::shared_ptr<Client>& client : clients)
	{
		if (client->getNickname() == nick)
			return true;
	}
	return false;
}

bool hasOwnNick(const std::shared_ptr<Client>& client, const std::string& nick)
{
	return client->getNickname() == nick;
}

void Server::nick(std::string buf, int fd, int index)
{
	std::cout << "--------------- NICK -----------------" << std::endl;
	std::string nick = buf.substr(5);

	if (!_clients[index] || !_clients[index]->getIsAuthenticated())
	{
		if (!_clients[index])
			std::cerr << "Error: Client " << fd << " not found." << std::endl;
		else
			sendError("491 NICK :Password required to connect", fd);
		std::cout << "Authentication error" << std::endl;
		return;
	}

	if (hasOwnNick(_clients[index], nick))
		return ;

	if (nick == "Nickname1is already in use" || isNickTaken(_clients, nick))
	{
		sendError("433 " + _clients[index]->getNickname() + " " + nick + " :Nickname is already in use", fd);
		return ;
	}

	if (!isValidNick(nick))
	{
		sendError("432 " + _clients[index]->getNickname() + " " + nick + " :Erroneus nickname", fd);
		return ;
	}

	sendResponse(":" + _clients[index]->getNickname() + " NICK " + nick, fd);

	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->isClient(_clients[index]) != -1)
		{
			_channels[i]->broadcast(":" + _clients[index]->getNickname() + " NICK " + nick + "\r\n", _clients[index], 1);
			_channels[i]->getClient(_channels[i]->isClient(_clients[index]))->setNickname(nick);
		}
	}

	_clients[index]->setNickname(nick);
}
