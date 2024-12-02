#include "Server.hpp"

Channel *Server::findChannel(const std::string &name)
{
	for (size_t i = 0 ; i < _channels.size() ; i++)
	{
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return nullptr;
}

Channel *Server::createChannel(const std::string &name, Client *creator, int fd)
{
	std::cout << "--------------- CREATE CHANNEL -----------------" << std::endl;

	if (name.empty() || name[0] != '#')
	{
		if (name.empty())
			sendError("461 JOIN :Not enough parameters", fd);
		else
			sendError("403 " + name + " :No such channel", fd);
		std::cout << "Name error" << std::endl;
		return nullptr;
	}

	if (name.empty() || !creator)
	{
		if (name.empty())
			sendError("461 JOIN :Not enough parameters", fd);
		else
			sendError("401 JOIN :No such name client found", fd);
		std::cout << "Name error 2" << std::endl;
		return nullptr;
	}

	Channel *channel = findChannel(name);
	if (channel != nullptr)
	{
		std::cout << "Channel " << name << " already exists." << std::endl;
		sendError("403 :Channel already exists, unable to create it again", fd);
		return nullptr;
	}

	std::cout << "Created a new channel: " << name << std::endl;
	Channel *newChannel = new Channel(name);

	newChannel->addClient(creator);
	std::string joinMsg = ":" + creator->getNickname() + " JOIN " + channel->getName() + "\r\n";
	send(creator->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);

	newChannel->addAdmin(creator);
	std::string response = "";
	if (creator->getNickname() !=  channel->getName())
		response += ":" + creator->getNickname() + " ";
	response += "PRIVMSG " + channel->getName() + " :I am now an admin of " + channel->getName() + "!\n";
	sendResponse(response, fd);

	_channels.push_back(newChannel);
	std::cout << *newChannel << std::endl;

	return newChannel;
}
