#include "Server.hpp"

void Server::join(std::string buf, int fd, int index)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	
	// check if client exists and is registerd
	if (!_clients[index] || !_clients[index]->getIsRegistered())
	{
		if (!_clients[index])
			std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		else
			sendError("451: You must register before using this command", fd);
		return;
	}

	// buf.replace(buf.find("\r"), 1, "");
	// buf.replace(buf.find("\n"), 1, "");

	std::string chName = buf.substr(5);
	if (chName.empty() || chName[0] != '#')
	{
		if (chName.empty())
			sendError("461: Not enough parameters for JOIN", fd);
		else
			sendError("401: Invalid channel name", fd);
		return;
	}

	std::cout << "Client " << _clients[index]->getNickname() << " is trying to join channel " << chName << std::endl;
	Channel *channel = findChannel(chName);
	if (channel == nullptr)
	{
		std::cout << "Channel named " << chName << " does not exist, creating channel now!" << std::endl;
		channel = createChannel(chName, _clients[index], fd);
		if (channel == nullptr) // if the creating of a channel not successful
		{
			sendError("Unable to join channel", fd);
			return;
		}
	}
	else if (!channel->isInvited(_clients[index]))
	{
		sendError("Channel is invite only", fd);
		return;
	}

	std::string joinMsg = ": " + _clients[index]->getNickname() + " JOIN " + channel->getName() + "\r\n";
	if (isInChannel(_clients[index]) == -1)
	{
		channel->addClient(_clients[index]);
		channel->broadcast(joinMsg);
	}

	std::cout << "Client " << _clients[index]->getNickname() << " joined channel " << channel->getName() << std::endl;
	std::cout << '\n' << *channel << std::endl;
}

