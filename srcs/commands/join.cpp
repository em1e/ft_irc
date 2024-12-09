#include "Server.hpp"

void Server::join(std::string buf, int fd, int index)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}
	std::string chName = buf.substr(5);
	if (chName.empty() || chName[0] != '#')
	{
		if (chName.empty())
			sendError("461 JOIN :Not enough parameters for JOIN", fd);
		else
			sendError("401 " + chName + " :Invalid channel name", fd);
		if (chName == ":")
			std::cout << "initial join" << std::endl;
		else
			std::cout << "Name error" << std::endl;
		return;
	}
	std::cout << "Client " << _clients[index]->getNickname() << " is trying to join channel " << chName << std::endl;
	Channel *channel = findChannel(chName);
	if (channel == nullptr)
	{
		std::cout << "Channel named " << chName << " does not exist, creating channel now!" << std::endl;
		channel = createChannel(chName, _clients[index], fd);
		if (channel == nullptr)
		{
			sendError("403 JOIN :Unable to create channel", fd);
			std::cout << "Channel create error" << std::endl;
			return;
		}

		std::string joinMsg = ":" + _clients[index]->getNickname() + " JOIN " + channel->getName() + "\r\n";
		send(_clients[index]->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);

		std::string response = "";
		if (_clients[index]->getNickname() !=  channel->getName())
			response += ":" + _clients[index]->getNickname() + " ";
		response += "PRIVMSG " + channel->getName() + " :I am now an admin of " + channel->getName() + "!\n";
		sendResponse(response, fd);
	}
	else if (channel->getInviteOnly() && !channel->isInvited(_clients[index]))
	{
		sendError("473 " + chName + " :Cannot join channel (+i)", fd);
		std::cout << "Channel join error" << std::endl;
		return;
	}

	if (!isInChannel(_clients[index], channel))
	{
		std::string joinMsg = ":" + _clients[index]->getNickname() + " JOIN " + channel->getName() + "\r\n";
		std::cout << "Client " << _clients[index]->getNickname() << " is joining channel " << channel->getName() << std::endl;
		std::cout << "Broadcasting join message: " << joinMsg << std::endl;
		channel->addClient(_clients[index]);
		channel->broadcast(joinMsg, nullptr, 0);
		if (channel->getTopic().empty())
			sendResponse(":localhost 331 " + _clients[index]->getNickname() + " " + channel->getName() + " :No topic is set", fd);
		else
			sendResponse(":localhost 332 " + _clients[index]->getNickname() + " " + channel->getName() + " :" + channel->getTopic(), fd);
		joinMsg = ":localhost 353 " + _clients[index]->getNickname() + " " + channel->getName() + " :";
		for (size_t i = 0; i < channel->getClients().size(); i++)
		{
			// send reply
		}

		// If a JOIN is successful, the user is then sent the channel's topic
//    (using RPL_TOPIC) and the list of users who are on the channel (using
//    RPL_NAMREPLY), which must include the user joining.

	}

	std::cout << *channel << std::endl;
}

