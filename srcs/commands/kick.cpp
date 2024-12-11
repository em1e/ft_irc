#include "Server.hpp"

void Server::kick(std::string buf, int fd, int index)
{
	std::cout << "--------------- KICK -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}

	std::istringstream iss(buf);
	std::string command, kick, chName, reason;
	iss >> command >> chName >> kick;

	reason = buf.substr(buf.find(kick) + kick.length(), buf.length() - (command.length() + chName.length() + kick.length() + 4));
	size_t pos = reason.find(":");
	if (pos != std::string::npos)
		reason = reason.substr(pos + 1);

	int clientIndex = searchByNickname(kick);
	if (kick.empty() || clientIndex == -1)
	{
		if (kick.empty())
			sendError("401 KICK :No target given", fd);
		else
			sendError("401 " + kick + " :No such client found", fd);
		std::cout << "Name error" << std::endl;
		return;
	}

	Channel *channel = findChannel(chName);
	if (chName.empty() || !channel || channel->isAdmin(_clients[index]) == -1
		|| channel->isClient(_clients[clientIndex]) == -1)
	{
		if (chName.empty())
			sendError("461 KICK :Not enough parameters", fd);
		else if (!channel)
			sendError("403 " + chName + " :No such channel exists", fd);
		else if (channel->isAdmin(_clients[index]) == -1)
			sendError("482 KICK :You're not channel admin", fd);
		else
			sendError("442 KICK :Client is not part of this channel", fd);
		std::cout << "Channel error" << std::endl;
		return;
	}

	if (channel->isAdmin(_clients[clientIndex]) != -1)
	{
		sendError("442 " + kick + " :You cannot kick an admin", _poll.getFd(clientIndex + 1).fd);
		std::cout << "Admin error" << std::endl;
		return;
	}

	channel->removeClient(_clients[clientIndex]);
	sendResponse(":" + _clients[index]->getNickname() + "!" + _clients[index]->getUsername() + "@" + std::to_string(_clients[clientIndex]->getIpAddress()) + " KICK " + channel->getName() + " " + _clients[clientIndex]->getNickname() + " :" + reason, fd);
	sendResponse(":localhost NOTICE " + _clients[index]->getNickname() + " :You have successfully kicked " + _clients[clientIndex]->getNickname() + " from " + channel->getName(), fd);
	sendResponse(":" + _clients[index]->getNickname() + "!" + _clients[index]->getUsername() + "@" + std::to_string(_clients[clientIndex]->getIpAddress()) + " KICK " + channel->getName() + " " + _clients[clientIndex]->getNickname() + " :" + reason, _poll.getFd(clientIndex + 1).fd);
	std::cout << *channel << std::endl;
}