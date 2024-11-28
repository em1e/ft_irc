#include "Server.hpp"

void Server::join(std::string buf, int fd)
{
	std::cout << "--------------- JOIN -----------------" << std::endl;
	Client *client = nullptr;
    for (Client *c : _clients) {
        if (c->getSocket() == fd) {
            client = c;
            break;
        }
    }
	if (!client) {
		std::cerr << "Error: Client: " << fd << " not found." << std::endl;
        return;
	}
	buf.replace(buf.find("\r"), 1, "");
	buf.replace(buf.find("\n"), 1, "");
	std::string chName = buf.substr(5);
	Channel *channel = createChannel(chName, client);
    std::string joinMsg = ":" + client->getNickname() + " JOIN " + channel->getName() + "\r\n";
    channel->broadcast(joinMsg, client); // Broadcast the join to the channel
    send(fd, joinMsg.c_str(), joinMsg.length(), 0); // Notify the client
    std::cout << "Client " << client->getNickname() << " joined channel " << channel->getName() << std::endl;
}

