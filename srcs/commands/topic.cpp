#include "Server.hpp"

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
	std::string notification = "Topic for channel " + _name + " changed to: " + topic;
	broadcastAdmins(notification);
	broadcast("The topic has been updated to: " + topic);
}

void Server::topic(std::string buf, int fd)
{
	std::cout << "--------------- TOPIC -----------------" << std::endl;
	Client *client = nullptr;
	for (Client *c : _clients) {
		if (c->getSocket() == fd) {
			client = c;
			break;
		}
	}
	if (!client) {
		std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		return;
	}
	buf.replace(buf.find("\r"), 1, "");
	buf.replace(buf.find("\n"), 1, "");
	std::istringstream iss(buf);
	std::string command, chName, newTopic;
	iss >> command >> chName;
	std::getline(iss, newTopic);
	Channel *channel = findChannel(chName);
	if (channel != nullptr)
	{
		if (newTopic.size() > 0 && newTopic[1] == ':')
			newTopic = newTopic.substr(2);
		if ((isInChannel(client) > -1) && channel->isAdmin(client))
			channel->setTopic(newTopic);
	}
	std::cout << '\n' << *channel << std::endl;
}


