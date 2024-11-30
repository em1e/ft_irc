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
			sendError("461 :Name empty", fd);
		else
			sendError("431 :Invalid channel name, must start with #", fd);
		return nullptr;
	}

	Channel *channel = findChannel(name);
	if (channel != nullptr)
	{
		std::cout << "Channel " << name << " already exists." << std::endl;
		sendError("403: Channel already exists, unable to create it again", fd);
		// we don't need this check since this func will only be called in join after all those checks
		// if (std::find(channel->getClients().begin(), channel->getClients().end(), creator) != channel->getClients().end())
		// 	sendError("You're already in the channel", fd);
		return nullptr;
	}

	Channel *newChannel = new Channel(name);
	std::cout << "created a new channel called " << name << std::endl;

	newChannel->addAdmin(creator);
	newChannel->addClient(creator);

	std::string joinMsg = ":" + creator->getNickname() + " JOIN " + newChannel->getName() + "\r\n";
	newChannel->broadcast(joinMsg);

	_channels.push_back(newChannel);

	return newChannel;
}
