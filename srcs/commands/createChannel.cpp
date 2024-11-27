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
        return channel;
    Channel *newChannel = new Channel(name);    
    newChannel->addClient(creator);
    newChannel->addAdmin(creator);
    _channels.push_back(newChannel);
    return newChannel;
}
