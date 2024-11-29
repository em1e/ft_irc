#include "Server.hpp"

void Server::join(std::string buf, int fd)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	Client *client = getClient(getNickname(fd));
	std::cout << "buf = " << buf << std::endl;

	if (!client)
	{	std::cerr << "Error: Client: " << fd << " not found." << std::endl; return;}

	size_t pos;
	if ((pos = buf.find("\r")) != std::string::npos)
		buf.replace(pos, 1, "");
	if ((pos = buf.find("\n")) != std::string::npos)
		buf.replace(pos, 1, "");

	std::string chName = buf.substr(5);
	if (chName[0] != '#')
	{	std::cout << "Error: channel name is missing #" << std::endl; return;}

	std::cout << "Client " << client->getNickname() << " is trying to join channel " << chName << std::endl;
	
	Channel *channel = findChannel(chName);
	if (channel == nullptr) // is channel found or not
	{
		std::cout << "Channel named " << chName << " does not exist, creating channel now!" << std::endl;
		channel = createChannel(chName, client);
		if (channel == nullptr) // was the creating of a channel not successful
		{
			std::string errorMsg = "Error: Unable to join channel " + chName + "\r\n";
			send(fd, errorMsg.c_str(), errorMsg.length(), 0);
			return;
		}
	}
	else if (!channel->isInvited(client))
	{
		std::cerr << "Error: Client " << client->getNickname() << " is not invited to channel " << chName << std::endl;
		std::string errorMsg = "Error: You are not invited to join channel " + chName + "\r\n";
		send(fd, errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}
	
	if (isInChannel(client) == -1)
		channel->addClient(client);
	std::string joinMsg = ":" + client->getNickname() + " JOIN " + channel->getName() + "\r\n";
	channel->broadcast(joinMsg);
	std::cout << "Client " << client->getNickname() << " joined channel " << channel->getName() << std::endl;
	std::cout << '\n' << *channel << std::endl;
}
