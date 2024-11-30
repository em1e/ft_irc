#include "Server.hpp"

void Server::privmsg(std::string buf, int fd, int index)
{
	std::cout << "--------------- PRIVMSG -----------------" << std::endl;

	// check if client exists and is registerd
	if (!_clients[index] || !_clients[index]->getIsRegistered())
	{
		if (!_clients[index])
			std::cerr << "Error: Client: " << fd << " not found." << std::endl;
		else
			sendError("451: You have not registered", fd);
		return;
	}

	buf.replace(buf.find("PRIVMSG "), 8, "");
	size_t pos = buf.find(" :");
	
	std::string name = buf.substr(0, pos);
	std::string msg = buf.substr(pos + 2);
	std::string response = "";
	// std::cout << "Target name: |" << name << "|" << std::endl;
	// std::cout << "Message: |" << msg << "|" << std::endl;
	
	if (name.empty())
	{
		sendError("411: No recipient given PRIVMSG", fd);
		return;
	}
	
	if (searchByNickname(name) != -1 && name[0] != '#')
	{
		if (_clients[index]->getNickname() != name)
			response += ":" + _clients[index]->getNickname() + " ";
		response += "PRIVMSG " + name + " :" + msg;
		sendResponse(response, _poll.getFds()[searchByNickname(name) + 1].fd);
	}
	// handle sending messages into a channel
	else if (name[0] == '#')
	{
		int channelIndex = getChannelIndex(name);
		//  403     ERR_NOSUCHCHANNEL
        //                 "<channel name> :No such channel"

        //         - Used to indicate the given channel name is invalid.
		// WRONG CODE
		if (channelIndex == -1)
		{
			sendError("40: No such channel exists", fd);
			return;
		}

		std::string message = ":" + _clients[index]->getNickname() + " PRIVMSG " + name + ' ' + msg + "\r\n";
		std::cout << _clients[index]->getNickname() << " is broadcasting to " << name << " :" << message << std::endl;
		_channels[channelIndex]->broadcast(message);
	}
	else
		sendError("401" + name + ":No such nick/channel", fd);
}
