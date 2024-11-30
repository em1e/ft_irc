#include "Server.hpp"

void Server::sendResponse(std::string msg, int fd)
{
	std::string response = msg + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
}

void Server::sendError(std::string msg, int fd)
{
	std::string response = ":localhost " + msg + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
}

/*
	Returns the nickname of the client that has given clientsocket,
	otherwise empty string.
*/
std::string Server::getNickname(int fd)
{
	for (Client *client : _clients)
	{
		if (client->getSocket() == fd)
			return client->getNickname();
	}
	return "";
}

Client *Server::getClient(std::string nick)
{
	for (Client *client : _clients)
	{
		if (client->getNickname() == nick)
			return client;
	}
	return nullptr;
}

/*
	Loops through all _clients in Server class and compares the
	username given as an argument to them.

	Returns the index of the client with the same username,
	otherwise -1.
*/
int Server::searchByUsername(std::string user)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getUsername() == user)
			return i;
	}
	return -1;
}

/*
	Loops through all _clients in Server class and compares the
	nick given as an argument to them.

	Returns the index of the client with the same nickname,
	otherwise -1.
*/
int Server::searchByNickname(std::string nick)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getNickname() == nick)
			return i;
	}
	return -1;
}

/*
	Loops through all clients in all of the channels
	and compares the client given as an argument to them.

	Returns the index of the channel a given client is in,
	otherwise -1.
*/
int Server::isInChannel(Client *client)
{
	//maybe we need to check if the client is found in the channel
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		for (Client *tmp_client : _channels[i]->getClients())
		{
			if (tmp_client == client)
				return i;
		}
	}
	return -1;
}

/*
	Loops through channels and fetchs the index of the channel
	with the same name as given argument.

	Returns the index of the channel with the same name,
	otherwise -1.
*/
int Server::getChannelIndex(std::string name)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->getName() == name)
			return i;
	}
	return -1;
}