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
		if (channel == nullptr) // if the creating of a channel not successful
		{
			sendError("403 JOIN :Unable to create channel", fd);
			std::cout << "Channel create error" << std::endl;
			return;
		}

		std::string joinMsg = ":" + _clients[index]->getNickname() + " JOIN " + channel->getName() + "\r\n";
		send(_clients[index]->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);

		// std::string response = "";
		// if (_clients[index]->getNickname() !=  channel->getName())
		// 	response += ":" + _clients[index]->getNickname() + " ";
		// response += "PRIVMSG " + channel->getName() + " :I am now an admin of " + channel->getName() + "!\n";
		// sendResponse(response, fd);

		// std::string msg = "- - - - - - - - " + channel->getName() + " - - - - - - - -\n" +
		// "Available commands:\n" +
		// "¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨\n" +
		// " -> \x02/nick\x02 <nickname> - Change your nickname\n"
		// " -> \x02/msg\x02 <nickname> <message> - Send a private message\n"
		// " -> \x02/join\x02 <#channel> - Join a channel\n"
		// "    -> \x02/topic\x02 <#channel> <topic> - Set a topic for the channel\n"
		// "    -> \x02/mode\x02 <#channel> <mode> (mode parameters) - Set a mode for the channel\n"
		// "    -> \x02/kick\x02 <#channel> <nickname> (reason) - Kick a user from the channel\n"
		// "    -> \x02/invite\x02 <nickname> <#channel> - Invite a user to the channel\n"
		// " -> \x02/quit\x02 - Disconnect from the server\n"
		// " -> \x02/list\x02 - Shows a list of channels\n";
		// std::cout << "msg: |" << msg << "|" << std::endl;
		// response = "";
		// if (_clients[index]->getNickname() !=  channel->getName())
		// 	response += ":" + _clients[index]->getNickname() + " ";
		// response += "PRIVMSG " + channel->getName() + " :" + msg;
		// std::cout << "Response: |" << response << "|" << std::endl;
		// sendResponse(response, fd);

	}
	else if (channel->getInviteOnly() && !channel->isInvited(_clients[index]))
	// else if (!channel->isInvited(_clients[index]))
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
	}

	std::cout << *channel << std::endl;
}

