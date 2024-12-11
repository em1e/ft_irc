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
			sendError("401 " + _clients[index]->getNickname() + " " + chName + " :Invalid channel name", fd);
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
		if (channel == nullptr)
		{
			sendError("403 JOIN :Unable to create channel", fd);
			std::cout << "Channel create error" << std::endl;
			return;
		}
	}
	else if (channel->getInviteOnly() && channel->isInvited(_clients[index]) == -1)
	{
		sendError("473 " + _clients[index]->getNickname() + " " + chName + " :Cannot join channel (+i)", fd);
		std::cout << "Channel join error" << std::endl;
		return;
	}

	if (!isInChannel(_clients[index], channel))
	{
		std::cout << "GOING IN HERE" << std::endl;
		std::string joinMsg = ":" + _clients[index]->getNickname() + "!" + _clients[index]->getUsername() + "@localhost JOIN " + channel->getName() + "\r\n";
		std::cout << "Client " << _clients[index]->getNickname() << " is joining channel " << channel->getName() << std::endl;
		std::cout << "Broadcasting join message: " << joinMsg << std::endl;
		channel->addClient(_clients[index]);
		channel->broadcast(joinMsg, nullptr, 0);
		if (channel->getTopic().empty())
			sendResponse(":localhost 331 " + _clients[index]->getNickname() + " " + channel->getName() + " :No topic is set", fd);
		else
		{
			sendResponse(":localhost 332 " + _clients[index]->getNickname() + " " + channel->getName() + " :" + channel->getTopic(), fd);
			
		}
		joinMsg = ":localhost 353 " + _clients[index]->getNickname() + " = " + channel->getName() + " :@";

		for (const auto& clientPtr : channel->getClients())
		{
			std::cout << "client " << clientPtr->getNickname() << std::endl;
			if (clientPtr)
				joinMsg += clientPtr->getNickname() + " ";
		}
		joinMsg.pop_back();
		std::cout << "msg is |"<< joinMsg << "|" << std::endl;
		sendResponse(joinMsg, fd);
		sendResponse(":localhost 366 " + _clients[index]->getNickname() + " " + channel->getName() + " :End of /NAMES list", fd);
		sendResponse(":localhost 329 " + _clients[index]->getNickname()  + " " + channel->getName() + " " + channel->getCreationTime(), fd);
	}

	std::cout << *channel << std::endl;
}

