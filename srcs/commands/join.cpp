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
	
	std::istringstream iss(buf);
	std::string command, channelsStr, passwordsStr;
	std::vector<std::string> channels;
	std::vector<std::string> passwords;
	
	iss >> command >> channelsStr >> passwordsStr;
	
	// Split channel names by comma
	std::string channel;
	std::istringstream channelStream(channelsStr);
	while (std::getline(channelStream, channel, ','))
		channels.push_back(channel);

	// Split passwords by comma
	std::string password;
	std::istringstream passwordStream(passwordsStr);
	while (std::getline(passwordStream, password, ','))
		passwords.push_back(password);
	
	std::cout << "Client " << nick << " is trying to join channel " << channelsStr << std::endl;
	for (size_t i = 0; i < channels.size(); ++i)
	{
		std::string chName = channels[i];
		// Check for valid channel name
		if (chName.empty() || chName[0] != '#')
		{
			if (chName.empty())
				sendError("461 " + nick + " JOIN :Not enough parameters", fd);
			else
				sendError("403 " + nick + " " + chName + " :No such channel", fd);
			continue;
		}
		
		Channel *channel = findChannel(chName);
		// If the channel doesn't exist, create it
		if (channel == nullptr)
		{
			channel = createChannel(chName, _clients[index], fd);
			if (channel == nullptr)
				continue;
		}
		// Check if channel is invite only
		if (channel->getInviteOnly() && channel->isInvited(_clients[index]) == -1)
		{
			sendError("473 " + nick + " " + chName + " :Cannot join channel (+i)", fd);
			continue;
		}
		// Check if channel is full
	
		
		// Check if passwordprotected
		if (channel->getIsChannelPassword() && !passwords[i].empty() && channel->getPassword() != passwords[i])
		{
			sendError("475 " + nick + " " + chName + " :Cannot join channel (+k)", fd);
			continue;
		}
		// std::string channelPassword = passwords[i];

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
}

