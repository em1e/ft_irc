#include "Server.hpp"

Channel *Server::findChannel(const std::string &name)
{
	for (size_t i=0 ; i < _channels.size() ; i++)
	{
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return nullptr;
}

Channel *Server::createChannel(const std::string &name, Client *creator)
{
	std::cout << "--------------- CREATE CHANNEL -----------------" << std::endl;
	std::cout << "name : |" << name << "|" << std::endl;
	if (name.empty() || name[0] != '#')
		throw std::invalid_argument("Invalid channel name: Must start with #.");
	
	Channel *channel;
	channel = findChannel(name);
	if (channel != nullptr)
	{
		std::cout << "Channel " << name << " already exists." << std::endl;
		if (std::find(channel->getClients().begin(), channel->getClients().end(), creator) != channel->getClients().end())
		{
			std::string errorMsg = "Error: You are already in the channel.\r\n";
			send(creator->getSocket(), errorMsg.c_str(), errorMsg.length(), 0);
			return nullptr;
		}
		return nullptr;
	}

	Channel *newChannel = new Channel(name);
	std::cout << "creating new channel called " << name << std::endl;
	try {
		std::cout << "Adding client to channel" << std::endl;
		newChannel->addClient(creator);
		std::cout << "Adding admin to channel" << std::endl;
		newChannel->addAdmin(creator);
	}
	catch (const std::exception &e){
		std::string msg = "Error: " + std::string(e.what()) + "\r\n";
		send(creator->getSocket(), msg.c_str(), msg.length(), 0);
		delete newChannel;
		return nullptr;
	}
	_channels.push_back(newChannel);
	return newChannel;
}
