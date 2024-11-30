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

	// std::cout << "buf = " << buf << std::endl;
	std::istringstream iss(buf);
	std::string command, name, msg;
	iss >> command >> name;

	msg = buf.substr(buf.find(name) + name.length() ,buf.length() - (command.length() + name.length() + 3));
	size_t pos = msg.find(":");
	if (pos != std::string::npos)
		msg = msg.substr(pos + 1);
	// msg.replace(msg.find("\r"), 1, "");
	// msg.replace(msg.find("\n"), 1, "");

	// std::cout << "command: " << command << std::endl;
	// std::cout << "name: " << name << std::endl;
	// std::cout << "msg: " << msg << std::endl;
	
	if (name.empty())
	{
		sendError("411: No recipient given PRIVMSG", fd);
		return;
	}

	// handle messages from a user to another
	int clientIndex = searchByNickname(name);
	if (clientIndex != -1 && name[0] != '#')
	{
		std::cout << "Client " << _clients[index]->getNickname() << " has messaged " << name << ": " << msg << std::endl;
		
		std::string response = ":" + _clients[index]->getNickname() + " PRIVMSG " + name + " :" + msg + "\r\n";
		sendResponse(response, fd);
		sendResponse(response, _poll.getFds()[clientIndex + 1].fd);
		// send(fd, response.c_str(), response.length(), 0);
		// send(_poll.getFds()[clientIndex + 1].fd, response.c_str(), response.length(), 0);
		// open_irssi_window(response);
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
