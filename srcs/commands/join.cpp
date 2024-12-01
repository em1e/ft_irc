#include "Server.hpp"

void Server::join(std::string buf, int fd, int index)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}

	std::string chName = buf.substr(5);
	if (chName.empty() || chName[0] != '#')
	{
		if (chName.empty())
			sendError("461 JOIN :Not enough parameters for JOIN", fd);
		else
			sendError("401 " + chName + " :Invalid channel name", fd);
		if (chName == ":")
			std::cout << "initial join" << std::endl;
		else
			std::cout << "Name error" << std::endl;
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
			sendError("403 JOIN :Unable to create channel", fd);
			std::cout << "Channel create error" << std::endl;
			return;
		}
	}
	else if (!channel->isInvited(_clients[index]))
	{
		sendError("473 " + chName + " :Cannot join channel (+i)", fd);
		std::cout << "Channel join error" << std::endl;
		return;
	}

	if (!isInChannel(_clients[index], channel))
	{
		std::string joinMsg = ":" + _clients[index]->getNickname() + " JOIN " + channel->getName() + "\r\n";
		// std::string joinMsg = ":" + client->getNickname() + " JOIN " + channel->getName() + "\r\n";
		std::cout << "Client " << _clients[index]->getNickname() << " is joining channel " << channel->getName() << std::endl;
		std::cout << "Broadcasting join message: " << joinMsg << std::endl;
		channel->addClient(_clients[index]);
		channel->broadcast(joinMsg, nullptr, 0);
	}

	std::cout << "Client " << _clients[index]->getNickname() << " joined channel " << channel->getName() << std::endl;
	std::cout << *channel << std::endl;
}

