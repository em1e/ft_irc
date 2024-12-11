#include "Server.hpp"

// void Channel::setTopic(const std::string &topic, Client *admin)
void Channel::setTopic(const std::string &topic, const std::shared_ptr<Client>& admin)
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

	std::istringstream iss(buf);
	std::string command, chName, newTopic;
	iss >> command >> chName;

	Channel *channel = findChannel(chName);
	if (chName.empty() || !channel)
	{
		std::cout << "command" << command << std::endl;
		std::cout << "chName" << chName << std::endl;
		if (chName.empty())
			sendError("461 " + _clients[index]->getNickname() + " " + chName + " :Not enough parameters", fd);
		else
			sendError("401 " + _clients[index]->getNickname() + " " + chName + " :No such channel", fd);
		std::cout << "Channel error" << std::endl;
		return;
	}

	if (channel->isClient(_clients[index]) == -1)
	{
		sendError("442 " + channel->getName() + " :You're not on that channel", fd);
		return;
	}

	if (channel->getTopicRestrictions() && channel->isAdmin(_clients[index]) == -1)
	{
		sendError("482 "  + _clients[index]->getNickname() + " " + chName + " :You're not channel operator", fd);
		std::cout << "Topic restrictions" << std::endl;
		return;
	}

	std::getline(iss, newTopic);
	if (!newTopic.empty())
	{
		if (newTopic.size() > 0 && newTopic[1] == ':')
			newTopic = newTopic.substr(2);
		channel->setTopic(newTopic, _clients[index]);
		sendReply("332 " + _clients[index]->getNickname() + " " + channel->getName() + " :" + newTopic, fd);
		std::string response = ":localhost 332 " + _clients[index]->getNickname() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n";
		channel->broadcast(response, nullptr, 0);
		channel->broadcastAdmins(response);
	}
	else
	{
		sendResponse(":localhost 331 " + _clients[index]->getNickname() + " " + channel->getName() + " :No topic is set", fd);
		std::cout << "No topic is set" << std::endl;
	}

	std::cout << *channel << std::endl;
}


