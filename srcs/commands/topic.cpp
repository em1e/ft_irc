#include "Server.hpp"

void Channel::setTopic(const std::string &topic, Client *admin) {
    if (!isAdmin(admin)) {
        admin->sendMessage("You do not have permission to set the topic.");
        return;
    }
    if (topic != _topic)
    {    _topic = topic;
        std::string notification = "Topic for channel " + _name + " changed to: " + topic;
        broadcastAdmins(notification);
        broadcast("The topic has been updated to: " + topic, admin);
    }
}

void Server::topic(std::string buf, int fd)
{
	std::cout << "--------------- TOPIC -----------------" << std::endl;
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
    ///TOPIC [channel_name] [new topic]
    std::istringstream iss(buf);
    std::string command, chName, newTopic;
    iss >> command >> chName;
    std::getline(iss, newTopic);
    Channel *channel = createChannel(chName, client);
    if (newTopic.size() > 0 && newTopic[0] == ':')
        newTopic = newTopic.substr(1);
    channel->setTopic(newTopic, client);
}


