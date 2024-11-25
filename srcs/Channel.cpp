#include "Channel.hpp"

Channel::Channel(){}

Channel::Channel(std::string const &name): _name(name), _topic("") {}
// Channel::Channel(Channel const &a_copy){}

Channel::~Channel(){}

Channel &Channel::operator=(Channel const &){ return *this; }

std::string Channel::getName() const{ return _name; }

std::string Channel::getTopic() const{ return _topic; }

Client *Channel::getAdmin() const{ return _admin; }

void Channel::setAdmin(Client *client){ _admin = client; }

void Channel::addClient(Client *client){ _clients.push_back(client); }

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