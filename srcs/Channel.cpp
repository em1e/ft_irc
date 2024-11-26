#include "Channel.hpp"

Channel::Channel(){}
Channel::Channel(std::string){}
Channel::Channel(Channel const &){}
Channel::~Channel(){}
Channel &Channel::operator=(Channel const &){ return *this; }

void Channel::addClient(Client *client)
{ 
	_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i] == client)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}