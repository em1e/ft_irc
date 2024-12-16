#include "Server.hpp"

void Server::privmsg(std::string buf, int fd, int index)
{
	std::cout << "--------------- PRIVMSG -----------------" << std::endl;

	if (!validateClientRegistration(fd, index))
	{
		std::cout << "Authentication error" << std::endl;
		return ;
	}

	buf.replace(buf.find("PRIVMSG "), 8, "");
	size_t pos = buf.find(" :");
	
	std::string name = buf.substr(0, pos);
	std::string msg = buf.substr(pos + 2);
	std::string response = "";
	std::cout << "Target name: |" << name << "|" << std::endl;
	std::cout << "Message: |" << msg << "|" << std::endl;
	
	if (name.empty())
	{
		sendError("411 :No recipient given PRIVMSG", fd);
		std::cout << "Recipient error" << std::endl;
		return;
	}
	
	if (searchByNickname(name) != -1 && name[0] != '#')
	{
		if (_clients[index]->getNickname() != name)
			response += ":" + _clients[index]->getNickname() + " ";
		response += "PRIVMSG " + name + " :" + msg;
		sendResponse(response, _poll.getFds()[searchByNickname(name) + 1].fd);
		std::cout << _clients[index]->getNickname() << " is sending a private message to " << name << " :" << response << std::endl;
	}
	else if (name[0] == '#')
	{
		int channelIndex = getChannelIndex(name);
		if (channelIndex == -1)
		{
			sendError("403 " + name + " :No such channel", fd);
			std::cout << "Channel name error" << std::endl;
			return;
		}
		Channel *channel = findChannel(name);
		if (channel->isClient(_clients[index]) == -1)
		{
			sendError("404 " + _clients[index]->getNickname() + " " + name + " :Cannot send to nick/channel", fd);
			return;
		}
		std::string message = ":" + _clients[index]->getNickname() + " PRIVMSG " + name + ' ' + msg + "\r\n";
		std::cout << _clients[index]->getNickname() << " is broadcasting to " << name << " :" << message << std::endl;
		_channels[channelIndex]->broadcast(message, _clients[index], 1);
	}
	else
	{
		sendError("401 " + _clients[index]->getNickname() + " " + name + " :No such nick/channel", fd);
		std::cout << "Channel error" << std::endl;
	}
}
