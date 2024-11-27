#include "Server.hpp"

void Server::privmsg(std::string buf, std::string response, int fd, int index)
{
	std::cout << "--------------- PRIVMSG -----------------" << std::endl;
	buf.replace(buf.find("\r"), 1, "");
	buf.replace(buf.find("\n"), 1, "");
	std::cout << "Buffer before ':' |" << buf << "|" << std::endl;
	buf.replace(buf.find("PRIVMSG "), 8, "");
	std::string name = buf.substr(0, buf.find(" "));
	size_t pos = buf.find(":");
	if (pos != std::string::npos)
		buf = buf.substr(pos + 1);
	// std::cout << "Buffer after ':' |" << buf << "|" << std::endl;
	// std::cout << "Name: |" << name << "|" << std::endl;
	if (searchByNickname(name) != -1)
	{
		std::cout << "Name: |" << name << "|" << std::endl;
		std::cout << "Client " << _clients[index - 1]->getNickname() << " has messaged " << name << ": " << buf << std::endl;
		response = ":localhost 001 PRIVMSG " + _clients[index - 1]->getNickname() + " -> " + name + " :" + buf + "\r\n";
		send(fd, response.c_str(), response.length(), 0);
		send(_poll.getFds()[searchByNickname(name) + 1].fd, response.c_str(), response.length(), 0);
	}
	else
	{
		std::cout << "Name: |" << name << "|" << std::endl;
		std::cout << "Client " << _clients[index - 1]->getNickname() << " has tried to message " << name << ": " << buf << std::endl;
		response = ":localhost 001 " + name + " : No such nick found\r\n";
		send(fd, response.c_str(), response.length(), 0);
	}
}