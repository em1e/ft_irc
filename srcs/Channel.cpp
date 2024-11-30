#include "Channel.hpp"

Channel::Channel(const std::string &name)
	: _inviteOnly(false), _topicRestrictions(false), _isChannelPassword(false),
	_userLimit(-1), _name(name)  //, _topic(nullptr), _password(nullptr)
{
	std::cout << "Channel " << getName() << " has been created" << std::endl;
}

Channel::~Channel()
{
	// _clients.clear();
	// _admins.clear();
	// _invited.clear();

	std::cout << "Channel " << getName() << " destroyed." << std::endl;
}

void Channel::addClient(Client *client)
{ 
	if (getUserLimit() == -1 || static_cast<int>(_clients.size()) < getUserLimit())
	{
		_clients.push_back(client);
		std::cout << "client " << client->getNickname() << " has been added to " << getName() << std::endl;
		// broadcastAdmin(":" + client->getNickname() + " JOIN " + getName() + "\r\n");
		std::cout << *this << std::endl;
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
			broadcastAdmins(": " + client->getNickname() + " was removed " + getName() + "\r\n");
			break;
		}
	}
	std::cout << *this << std::endl;
}

bool Channel::isClient(Client *client) const
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i] == client)
			return true;
	}
	return false;
}

void Channel::addAdmin(Client *admin)
{
	if (isAdmin(admin))
	{
		std::cout << "Error: " + admin->getNickname() + " is already an admin of " << getName() << std::endl;
		return;
	}
	broadcastAdmins(": " + admin->getNickname() + " was added as admin of " + getName() + "\r\n");
	std::cout << admin->getNickname() << " was added as admin to " << getName() << std::endl;
	_admins.push_back(admin);
}

void Channel::removeAdmin(Client *admin)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		// check that admin being removed is not the last one
		if (_admins[i] == admin)
		{
			_admins.erase(_admins.begin() + i);
			broadcastAdmins(": " + admin->getNickname() + " was removed as admin of " + getName() + "\r\n");
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

void Channel::broadcast(const std::string &msg)
{
	for (size_t i = 0; i < _clients.size(); ++i)
		send(_clients[i]->getSocket(), msg.c_str(), msg.length(), 0);
}

void Channel::broadcastAdmins(const std::string &msg)
{
	std::cout << "broadcasting to admins" << std::endl;
	for (size_t i = 0; i < _admins.size(); ++i)
	{
		std::cout << "sending to admin " << _admins[i]->getNickname() << std::endl;
		send(_admins[i]->getSocket(), msg.c_str(), msg.length(), 0);
	}
}

void Channel::addInvited(Client *client)
{
	if (isInvited(client))
	{
		std::cout << "Error: " + client->getNickname() + " is already invited to " << getName() << std::endl;
		return;
	}
	_invited.push_back(client);
	std::cout << "adding " << client->getNickname() << " to the invited list" << std::endl;
	broadcastAdmins(": " + client->getNickname() + " has been invited to " + getName() + "\r\n");
}

void Channel::removeInvited(Client *client)
{
	for (size_t i = 0; i < _invited.size(); ++i)
	{
		if (_invited[i] == client)
		{	_invited.erase(_invited.begin() + i); break;}
	}
}

int Channel::isInvited(Client *client)
{
	if (!_inviteOnly)
		return 1;
	for (size_t i = 0; i < _invited.size(); ++i)
	{
		if (_invited[i] == client)
		{	std::cout << "client is not invited to " << _name << std::endl; return 1;}
	}
	return 0;
}

std::ostream& operator<<(std::ostream& os, const Channel& channel)
{
	os << "Channel: " << channel.getName() << "\n";
	os << "Topic: " << channel.getTopic() << "\n";
	os << "User Limit: " << (channel.getUserLimit() != -1 ? "yes" : "No") << "\n";
	os << "Invite Only: " << (channel.getInviteOnly() ? "Yes" : "No") << "\n";
	os << "Topic Restrictions: " << (channel.getTopicRestrictions() ? "Yes" : "No") << "\n";
	os << "Password Protected: " << (channel.getIsChannelPassword() ? "Yes" : "No") << "\n";

	os << "Admins:\n";
	for (const Client* admin : channel.getAdmins())
		os << "  - " << admin->getNickname() << "\n";

	os << "Clients:\n";
	for (const Client* client : channel.getClients())
		os << "  - " << client->getNickname() << "\n";

	os << "Invited Clients:\n";
	for (const Client* invited : channel.getInvited())
		os << "  - " << invited->getNickname() << "\n";

	return os;
}
