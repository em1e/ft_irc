#include "Server.hpp"

void Server::join(std::string buf, int fd, int index)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}
	
	std::string nick = _clients[index]->getNickname();
	std::string chName = buf.substr(5);
	
	 // Check for valid channel name
	if (chName.empty() || chName[0] != '#')
	{
		if (chName.empty())
			sendError("461 " + nick + " JOIN :Not enough parameters", fd);
		else
			sendError("403 " + nick + " " + chName + " :No such channel", fd);
		return;
	}
	
	std::cout << "Client " << nick << " is trying to join channel " << chName << std::endl;
	Channel *channel = findChannel(chName);
	
	// If the channel doesn't exist, create it
	if (channel == nullptr)
	{
		channel = createChannel(chName, _clients[index], fd);
		if (channel == nullptr)
			return;
	}
	// Check if channel is invite only
	else if (channel->getInviteOnly() && channel->isInvited(_clients[index]) == -1)
	{
		sendError("473 " + nick + " " + chName + " :Cannot join channel (+i)", fd);
		return;
	}
	// Check if channel is full

	 // Check if client is already in the channel. If not, join
	if (!isInChannel(_clients[index], channel))
	{
		channel->addClient(_clients[index]);
		channel->broadcast(":" + nick + "!" + _clients[index]->getUsername() + "@localhost JOIN " + channel->getName() + "\r\n", nullptr, 0);
		if (!channel->getTopic().empty())
			sendResponse(":localhost 332 " + nick + " " + channel->getName() + " :" + channel->getTopic(), fd);
		std::string joinMsg = ":localhost 353 " + nick + " = " + channel->getName() + " :@";
		for (const auto& clientPtr : channel->getClients())
		{	
			if (channel->isAdmin(clientPtr) != -1)
				joinMsg += "@";
			if (clientPtr)
				joinMsg += clientPtr->getNickname() + " ";
		}
		joinMsg.pop_back();
		sendResponse(joinMsg, fd);
		sendResponse(":localhost 366 " + nick + " " + channel->getName() + " :End of /NAMES list", fd);
		sendResponse(":localhost 329 " + nick  + " " + channel->getName() + " " + channel->getCreationTime(), fd);
	}
	std::cout << *channel << std::endl;
}

