#include "Server.hpp"

void Server::join(std::string buf, int fd, int index)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
		return ;

	std::string chName = buf.substr(5);
	if (chName.empty() || chName[0] != '#')
	{
		if (chName.empty())
			sendError("461 :Not enough parameters for JOIN", fd);
		else
			sendError("401 :Invalid channel name", fd);
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
			sendError("403 :Unable to join channel", fd);
			return;
		}
	}
	else if (!channel->isInvited(_clients[index]))
	{
		sendError("473 " + chName + " :Cannot join channel (+i)", fd);
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

