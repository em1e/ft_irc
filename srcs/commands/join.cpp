#include "Server.hpp"

void Server::join(std::string buf, int fd)
{
	// handle /join command (check if !_clients[index].get_nickname().empty())
		// else handle it as client joining for the first time
	std::cout << "--------------- JOIN -----------------" << std::endl;
	Client *client = nullptr;
    for (Client *c : _clients) {
        if (c->getSocket() == fd) {
            client = c;
            break;
        }
    }
	if (!client) {
		std::cerr << "Error: Client not found for fd " << fd << std::endl;
        return;
	}
	buf.replace(buf.find("\r"), 1, "");
	buf.replace(buf.find("\n"), 1, "");
	std::string chName = buf.substr(5);
	Channel *channel = createChannel(chName, client);
	if (find(channel->getClients().begin(), channel->getClients().end(), client) != channel->getClients().end())
	{

	}
		



	std::cout << "Client " << client->getSocket() << " sent join message" << std::endl;
	// response += "JOIN :\r\n";
	// send(fd, response.c_str(), response.length(), 0);
}

// void Server::join(std::string response, int fd)
// {
//     // Check if the client is already in the channel
//     if (std::find(channel->getClients().begin(), channel->getClients().end(), client) != channel->getClients().end()) {
//         std::string errorMsg = "Error: You are already in the channel.\r\n";
//         send(fd, errorMsg.c_str(), errorMsg.length(), 0);
//         return;
//     }

//     // Add the client to the channel
//     try {
//         channel->addClient(client);
//     } catch (const std::exception &e) {
//         std::string errorMsg = "Error: " + std::string(e.what()) + "\r\n";
//         send(fd, errorMsg.c_str(), errorMsg.length(), 0);
//         return;
//     }

//     // Notify the client and the channel
//     std::string joinMsg = ":" + client->getNickname() + " JOIN " + channelName + "\r\n";
//     channel->broadcast(joinMsg, nullptr); // Broadcast the join to the channel
//     send(fd, joinMsg.c_str(), joinMsg.length(), 0); // Notify the client

//     std::cout << "Client " << client->getNickname() << " joined channel " << channelName << std::endl;
// }