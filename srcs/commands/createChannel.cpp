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

	// std::cout << "name : |" << name << "|" << std::endl;
	if (name.empty() || name[0] != '#')
	{
		if (name.empty())
			sendError("461 JOIN :Not enough parameters", fd);
		else
			sendError("403 " + name + " :No such channel", fd);
		std::cout << "Name error" << std::endl;
		return nullptr;
	}

	// Channel *channel = findChannel(name);
	// if (channel != nullptr)
	// {
	// 	std::cout << "Channel " << name << " already exists." << std::endl;
	// 	sendError("403 :Channel already exists, unable to create it again", fd);
	// 	// we don't need this check since this func will only be called in join after all those checks
	// 	// if (std::find(channel->getClients().begin(), channel->getClients().end(), creator) != channel->getClients().end())
	// 	// 	sendError("You're already in the channel", fd);
	// 	return nullptr;
	// }

	Channel *newChannel = new Channel(name);
	std::cout << "Created a new channel: " << name << std::endl;

	newChannel->addAdmin(creator);
	newChannel->addClient(creator);

	std::string joinMsg = ":" + creator->getNickname() + " JOIN " + newChannel->getName() + "\r\n";
	send(creator->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);
	send(creator->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);
	std::cout << "Client " << creator->getNickname() << " joined channel " << newChannel->getName() << std::endl;
	std::cout << "Broadcasting join message: " << joinMsg << std::endl;
	_channels.push_back(newChannel);

	return newChannel;
}
