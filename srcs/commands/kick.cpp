#include "Server.hpp"

void Server::kick(std::string buf, int fd, int index)
{
	std::cout << "--------------- KICK -----------------" << std::endl;
	
	// check if client exists and is registerd
	if (!_clients[index] || !_clients[index]->getIsRegistered())
	{
		if (!_clients[index])
			std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		else
			sendError("451: You must register before using this command", fd);
		return;
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
			sendError("401: No target given", fd);
		else
			sendError("401: No such client found", fd);
		return;
	}

	Channel *channel = findChannel(chName);
	if (chName.empty() || !channel || !channel->isAdmin(_clients[index])
		|| !channel->isClient(_clients[clientIndex]))
	{
		if (chName.empty())
			sendError("461: Not enough parameters for KICK", fd);
		else if (!channel)
			sendError("403: No such channel exists", fd);
		else if (!channel->isAdmin(_clients[index]))
			sendError("482: You're not channel admin", fd);
		else
			sendError("442: Client is not part of this channel", fd);
		return;
	}

	channel->removeClient(_clients[clientIndex]);
}