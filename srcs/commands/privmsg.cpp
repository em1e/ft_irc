#include "Server.hpp"

void Server::privmsg(std::string buf, int fd, int index)
{
	// std::cout << "--------------- PRIVMSG -----------------" << std::endl;
	// std::cout << "Buffer before ':' |" << buf << "|" << std::endl;
	
	buf.replace(buf.find("PRIVMSG "), 8, "");
	// std::cout << "Buffer after ':' |" << buf << "|" << std::endl;
	size_t pos = buf.find(" :");
	if (pos == std::string::npos)
	{
		std::cerr << "Invalid PRIVMSG format!" << std::endl;
		return;
	}
	
	std::string name = buf.substr(0, pos);
	std::string msg = buf.substr(pos + 2);
	std::string response = "";
	// std::cout << "Target name: |" << name << "|" << std::endl;
	// std::cout << "Message: |" << msg << "|" << std::endl;
	
	if (searchByNickname(name) != -1 && name[0] != '#')
	{
		if (_clients[index]->getNickname() != name)
			response += ":" + _clients[index]->getNickname() + " ";
		response += "PRIVMSG " + name + " :" + msg;
		sendResponse(response, _poll.getFds()[searchByNickname(name) + 1].fd);
	}
	else if (name[0] == '#')
	{
		std::cout << "Name: |" << name << "|" << std::endl;

		if (getChannelIndex(name) == -1)
		{	std::cout << "Error: no channel with name |" << name << "| exists" << std::endl; return;}

		std::cout << "Client " << _clients[index - 1]->getNickname() << " has messaged " << name << ": " << buf << std::endl;
		
		Client *client = _clients[index];
		std::string message = ":" + client->getNickname() + " PRIVMSG " + name + ' ' + msg + "\r\n";
		std::cout << "broadcasting :" << message << std::endl;
		_channels[getChannelIndex(name)]->broadcast(message);
	}
	else
	{
		std::cout << "Name: |" << name << "|" << std::endl;
		std::cout << "Client " << _clients[index]->getNickname() << " has tried to message " << name << ": " << buf << std::endl;
		sendResponse(":localhost 001 " + name + " : No such nick found", fd);
		// response = ":localhost 001 " + name + " : No such nickname or channel found\r\n";
		// send(fd, response.c_str(), response.length(), 0);
	}
}
