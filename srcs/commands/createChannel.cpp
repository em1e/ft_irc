#include "Server.hpp"

Channel *Server::findChannel(const std::string &name)
{
    for (size_t i=0 ; i < _channels.size() ; i++)
    {
        if (_channels[i]->getName() == name)
            return _channels[i];
        else
            return nullptr;
    }
}

Channel *Server::createChannel(const std::string &name, Client *creator)
{
    if (name.empty() || name[0] != '#')
        throw std::invalid_argument("Invalid channel name: Must start with #.");
    Channel *channel;
    channel = findChannel(name);
    if (channel != nullptr)
    {
        if (find(channel->getClients().begin(), channel->getClients().end(), creator->getUsername()) != channel->getClients().end())
	    {
		    std::string errorMsg = "Error: You are already in the channel.\r\n";
		    send(creator->getSocket(), errorMsg.c_str(), errorMsg.length(), 0);
		    return ;
	    }
        return channel;
    }
    Channel *newChannel = new Channel(name);  
    try {
        newChannel->addClient(creator);
        newChannel->addAdmin(creator);
    }
    catch (const std::exception &e){
        std::string msg = "Error: " + std::string(e.what()) + "\r\n";
        send(creator->getSocket(), msg.c_str(), msg.length(), 0);
        return;
    }
    _channels.push_back(newChannel);
    return newChannel;
}
