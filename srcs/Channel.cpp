#include "Channel.hpp"

Channel::Channel(const std::string &name)
	: _userLimit(-1), _name(name)
{
	std::cout << "Channel " << getName() << " has been created" << std::endl;
}

Channel::~Channel()
{
	// for (Client* client : _clients)
	// 	delete client;
	// _clients.clear();
	// for (Client* admin : _admins)
	// 	delete admin;
	// _admins.clear();
	// for (Client* invited : _invited)
	// 	delete invited;
	// _invited.clear();
}

void Channel::addClient(Client *client)
{ 
	if (getUserLimit() == -1 || static_cast<int>(_clients.size()) < getUserLimit())
	{
		_clients.push_back(client);
		std::cout << "client " << client->getNickname() << " has been added to " << getName() << std::endl;
		broadcastAdmin(":" + client->getNickname() + " JOIN " + getName() + "\r\n");
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
			broadcastAdmin(": " + client->getNickname() + " was removed " + getName() + "\r\n");
			break;
		}
	}
}

void Channel::addAdmin(Client *admin)
{
	_admins.push_back(admin);
	broadcastAdmin(": " + admin->getNickname() + " was added as admin of " + getName() + "\r\n");
	std::cout << admin->getNickname() << " was added as admin to " << getName() << std::endl;
}

void Channel::removeAdmin(Client *admin)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		// check that admin being removed is not the last one
		if (_admins[i] == admin)
		{
			_admins.erase(_admins.begin() + i);
			broadcastAdmin(": " + admin->getNickname() + " was removed as admin of " + getName() + "\r\n");
			break;
		}
	}
}

bool Channel::isAdmin(Client *client) const
{
	for (size_t i = 0; i < _admins.size(); ++i)
	{
		if (_admins[i] == client)
		{
			send(client->getSocket(), "Yes they are an admin of this channel.\r\n", 32, 0);
			return true;
		}
	}
	send(client->getSocket(), "No they are not an admin of this channel.\r\n", 33, 0);
	return false;
}

void Channel::broadcast(const std::string &msg, Client *sender)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i] != sender)
			send(_clients[i]->getSocket(), msg.c_str(), msg.length(), 0);
	}
}

void Channel::broadcastAdmin(const std::string &msg)
{
	std::cout << "broadcasting to admins" << std::endl;
	for (size_t i = 0; i < _admins.size(); ++i)
	{
		std::cout << "sending to admin " << _admins[i]->getNickname() << std::endl;
		send(_admins[i]->getSocket(), msg.c_str(), msg.length(), 0);
	}
}

void Channel::setTopic(const std::string &topic, Client *admin)
{
	if (isAdmin(admin))
	{
		_topic = topic;
		broadcastAdmin(": " + admin->getNickname() + " has set the topic of " + getName() + " to " + topic + "\r\n");
	}
}

void Channel::addInvited(Client *client)
{
	std::cout << "adding " << client->getNickname() << " to the invited list" << std::endl;
	broadcastAdmin(": " + client->getNickname() + " has been invited to " + getName() + "\r\n");
	_invited.push_back(client);
}

void Channel::removeInvited(Client *client)
{
	for (size_t i = 0; i < _invited.size(); ++i)
	{
		if (_invited[i] == client)
		{
			_invited.erase(_invited.begin() + i);
			break;
		}
	}
}

int Channel::isInvited(Client *client)
{
	for (size_t i = 0; i < _invited.size(); ++i)
	{
		if (_invited[i] == client)
		{
			std::cout << "client is not invited to " << _name << std::endl;
			return 1;
		}
	}
	return 0;
}

std::ostream& operator<<(std::ostream& os, const Channel& channel)
{
	os << "Channel: " << channel.getName() << "\n";
	os << "Topic: " << channel.getTopic() << "\n";
	os << "User Limit: " << channel.getUserLimit() << "\n";
	os << "Invite Only: " << (channel.getInviteOnly() ? "Yes" : "No") << "\n";
	os << "Topic Restrictions: " << (channel.getTopicRestrictions() ? "Yes" : "No") << "\n";
	os << "Password Protected: " << (channel.getIsChannelPassword() ? "Yes" : "No") << "\n";

	os << "Admins:\n";
	for (const Client* admin : channel.getAdmins())
	{
		os << "  - " << admin->getNickname() << "\n";
	}

	os << "Clients:\n";
	for (const Client* client : channel.getClients())
	{
		os << "  - " << client->getNickname() << "\n";
	}

	os << "Invited Clients:\n";
	for (const Client* invited : channel.getInvited())
	{
		os << "  - " << invited->getNickname() << "\n";
	}

	return os;
}
