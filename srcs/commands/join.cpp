#include "Server.hpp"

void Server::join(std::string buf, int fd)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	Client *client = nullptr;
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getSocket() == fd)
		{
			client = _clients[i];
			break;
		}
	}
	std::cout << "buf = " << buf << std::endl;

	if (!client)
	{
		std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		return;
	}

	size_t pos;
	if ((pos = buf.find("\r")) != std::string::npos)
		buf.replace(pos, 1, "");
	if ((pos = buf.find("\n")) != std::string::npos)
		buf.replace(pos, 1, "");

	std::string chName = buf.substr(5);
	if (chName[0] != '#')
	{
		std::cout << "Error: channel name is missing #" << std::endl;
		return;
	}

	std::cout << "Testing: " << chName << std::endl;
	std::cout << "Client " << client->getNickname() << " is trying to join channel " << chName << std::endl;
	
	Channel *channel = findChannel(chName);
	if (channel == nullptr)
	{
		channel = createChannel(chName, client);
		if (channel == nullptr)
		{
			std::cerr << "A Error: Unable to create channel " << chName << std::endl;
			std::string errorMsg = "Error: Unable to join channel " + chName + "\r\n";
			send(fd, errorMsg.c_str(), errorMsg.length(), 0);
			std::cout << "B Error: Unable to join channel " << chName << std::endl;
			return;
		}
	}
	
	// add this code when we have implemented mode
	// it makes it so that admins can invite only if inviteOnly is true

	// else if (!channel->isInvited(client))
	// {
	// 	std::cerr << "Error: Client " << client->getNickname() << " is not invited to channel " << chName << std::endl;
	// 	std::string errorMsg = "Error: You are not invited to join channel " + chName + "\r\n";
	// 	send(fd, errorMsg.c_str(), errorMsg.length(), 0);
	// 	return;
	// }

	std::string joinMsg = ":" + client->getNickname() + " JOIN " + channel->getName() + "\r\n";
	
	_channels.push_back(channel);
	channel->broadcast(joinMsg, client);
	send(fd, joinMsg.c_str(), joinMsg.length(), 0);
	std::cout << "Client " << client->getNickname() << " joined channel " << channel->getName() << std::endl;
}
