#include "Server.hpp"

void Server::privmsg(std::string buf, int fd, int index) {
	(void)buf;
	(void)fd;
	(void)index;
}
// {
// 	std::cout << "--------------- PRIVMSG -----------------" << std::endl;
// 	buf.replace(buf.find("\r"), 1, "");
// 	buf.replace(buf.find("\n"), 1, "");
// 	buf.replace(buf.find("PRIVMSG "), 8, "");
// 	// std::cout << "Buffer before ':' |" << buf << "|" << std::endl;
	
// 	std::string name = buf.substr(0, buf.find(" "));

	
// 	size_t pos = buf.find(":");

// 	std::istringstream iss(buf);
// 	std::string command, name, msg;
// 	iss >> command >> name >> msg;

// 	std::cout << "command: " << command << std::endl;
// 	std::cout << "name: " << name << std::endl;

// 	msg = buf.substr(buf.find(name) + name.length() ,buf.length() - (command.length() + name.length() + 3));

// 	size_t pos = msg.find(":");
// 	if (pos != std::string::npos)
// 		msg = msg.substr(pos + 1);

// 	std::cout << "msg: " << msg << std::endl;
	
// 	if (searchByNickname(name) != -1 && name[0] != '#')
// 	{
// 		std::cout << "Name: |" << name << "|" << std::endl;
// 		std::cout << "Client " << _clients[index - 1]->getNickname() << " has messaged " << name << ": " << buf << std::endl;
// 		// std::string response = ":localhost 001 PRIVMSG " + _clients[index - 1]->getNickname() + " -> " + name + " :" + buf + "\r\n";
// 		// :Alice PRIVMSG Bob :Hello Bob\r\n
// 		std::string response = ":" + _clients[index - 1]->getNickname() + " PRIVMSG " + name + " :" + buf + "\r\n";
// 		std::cout << response << std::endl;
// 		send(fd, response.c_str(), response.length(), 0);
// 		send(_poll.getFds()[searchByNickname(name) + 1].fd, response.c_str(), response.length(), 0);
// 		// open_irssi_window(response);
// 		// sendResponse(response, fd);
// 		// sendResponse(response, _poll.getFds()[searchByNickname(name) + 1].fd);
// 	}
// 	else if (name[0] == '#')
// 	{
// 		std::cout << "Name: |" << name << "|" << std::endl;

// 		if (getChannelIndex(name) == -1)
// 		{	std::cout << "Error: no channel with name |" << name << "| exists" << std::endl; return;}

// 		std::cout << "Client " << _clients[index - 1]->getNickname() << " has messaged " << name << ": " << buf << std::endl;
		
// 		Client *client = _clients[index - 1];
// 		std::string message = ":" + client->getNickname() + " PRIVMSG " + name + ' ' + msg + "\r\n";
// 		std::cout << "broadcasting :" << message << std::endl;
// 		_channels[getChannelIndex(name)]->broadcast(message);
// 	}
// 	else
// 	{
// 		std::cout << "Name: |" << name << "|" << std::endl;
// 		std::cout << "Client " << _clients[index - 1]->getNickname() << " has tried to message " << name << ": " << buf << std::endl;
// 		sendResponse(":localhost 001 " + name + " : No such nick found", fd);
// 		// response = ":localhost 001 " + name + " : No such nickname or channel found\r\n";
// 		// send(fd, response.c_str(), response.length(), 0);
// 	}
// }
