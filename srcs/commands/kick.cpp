#include "Server.hpp"

void Server::kick(std::string buf, int fd, int index)
{
	std::cout << "--------------- KICK -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}

	// std::cout << "admin : |" << _clients[index]->getNickname() << "|" << std::endl;
	// std::cout << "buf : |" << buf << "|" << std::endl;
	// buf.replace(buf.find("\r"), 1, "");
	// buf.replace(buf.find("\n"), 1, "");

	std::istringstream iss(buf);
	std::string command, kick, chName, reason;
	iss >> command >> chName >> kick;

	std::cout << "buf : |" << buf << "|" << std::endl;
	reason = buf.substr(buf.find(kick) + kick.length(), buf.length() - (command.length() + chName.length() + kick.length() + 4));
	size_t pos = reason.find(":");
	if (pos != std::string::npos)
		reason = reason.substr(pos + 1);
	std::cout << "reason : |" << reason << "|" << std::endl;
	// std::cout << "kick user : |" << kick << "|" << std::endl;

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
	if (chName.empty() || !channel || !channel->isAdmin(_clients[index])
		|| !channel->isClient(_clients[clientIndex]))
	{
		if (chName.empty())
			sendError("461 KICK :Not enough parameters", fd);
		else if (!channel)
			sendError("403 " + chName + " :No such channel exists", fd);
		else if (!channel->isAdmin(_clients[index]))
			sendError("482 KICK :You're not channel admin", fd);
		else
			sendError("442 KICK :Client is not part of this channel", fd);
		std::cout << "Channel error" << std::endl;
		return;
	}

	channel->removeClient(_clients[clientIndex]);
}