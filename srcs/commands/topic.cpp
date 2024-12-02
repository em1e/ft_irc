#include "Server.hpp"

void Channel::setTopic(const std::string &topic, Client *admin)
{
	_topic = topic;
	std::string notification = "Channel topic changed by " + admin->getNickname();
	broadcastAdmins(notification);
	broadcast("The topic has been updated to: " + topic, nullptr, 0);
}

void Server::topic(std::string buf, int fd, int index)
{
	std::cout << "--------------- TOPIC -----------------" << std::endl;
	
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}

	// buf.replace(buf.find("\r"), 1, "");
	// buf.replace(buf.find("\n"), 1, "");

	std::istringstream iss(buf);
	std::string command, chName, newTopic;
	iss >> command >> chName;

	Channel *channel = findChannel(chName);
	if (chName.empty() || !channel || !channel->isAdmin(_clients[index]))
	{
		if (chName.empty())
			sendError("461 : Not enough parameters for TOPIC", fd);
		else if (!channel)
			sendError("403 : No such channel exist", fd);
		else
			sendError("482 : You're not channel admin", fd);
		std::cout << "Channel error" << std::endl;
		return;
	}

	std::getline(iss, newTopic);
	if (!newTopic.empty())
	{
		if (newTopic.size() > 0 && newTopic[1] == ':')
			newTopic = newTopic.substr(2);
		channel->setTopic(newTopic, _clients[index]);
		std::string response = ":" + _clients[index]->getNickname() + " TOPIC " + chName + " :" + newTopic + "\r\n";
		channel->broadcast(response, nullptr, 0);
	}
	// give error if topic is nothing

	std::cout << *channel << std::endl;
}


