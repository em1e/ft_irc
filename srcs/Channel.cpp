#include "Channel.hpp"

Channel::Channel(const std::string &name) 
	: _name(name), _userLimit(-1)
{
	std::cout << "Channel " << getName() << " has been created" << std::endl;
}

Channel::~Channel(){}

void Channel::addClient(Client *client)
{ 
	if (getUserLimit() == -1 || _clients.size() < getUserLimit())
	{
		_clients.push_back(client);
		std::cout << "client " << client->getNickname() << " has been added to " << getName() << std::endl;
		// announce to the admins / operators that client x has been added to the channel
		return ;
	}
	std::cout << "cannot add client to the channel" << std::endl;
	
}

void Channel::removeClient(Client *client)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i] == client)
		{
			_clients.erase(_clients.begin() + i);
			// add announcement
			break;
		}
	}
}

void Channel::addAdmin(Client *admin){  _admins.push_back(admin); } // add announcement

void Channel::removeAdmin(Client *admin)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		// check that admin being removed is not the last one
		if (_admins[i] == admin)
		{
			_admins.erase(_admins.begin() + i);
			// add announcement
			break;
		}
	}
}

bool Channel::isAdmin(Client *client) const
{
	for (size_t i = 0; i < _admins.size(); ++i)
	{
		if (_admins[i] == client)
			return true;
	}
	return false;
}

