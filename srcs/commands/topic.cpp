#include "Server.hpp"

void Channel::setTopic(const std::string &topic, Client *admin) {
    if (!isAdmin(admin)) {
        admin->sendMessage("You do not have permission to set the topic.");
        return;
    }
    _topic = topic;
    std::string notification = "Topic for channel " + _name + " changed to: " + topic;
    broadcastToAdmins(notification); // Notify only admins
    broadcast("The topic has been updated to: " + topic, admin); // Notify all members
}


void Server::topic(std::string buf, int fd)
{
	// std::cout << "--------------- INVITE -----------------" << std::endl;
	// std::cout << "buffer before : |" << buf << "|"<< std::endl;
	// buf.replace(buf.find("INVITE"), 6, "");
	// std::cout << "buffer after : |" << buf << "|" << std::endl;

	// std::cout << "Client " << fd << " has invited" << std::endl;
}