#include "Server.hpp"

void Server::capLs(int fd, int index)
{
	std::cout << "--------------- CAP LS -----------------" << std::endl;
	if (!_clients[index - 1]->getIsRegistered()) {
        // Optionally, respond with an error that the user must register first
        sendError("451 :You have not registered", fd);
        return ;
    }
	sendResponse(":localhost CAP * LS :multi-prefix sasl away-notify", fd);
	sendResponse(":localhost 001 CAP LS : ", fd);
	// std::cout << "Client " << fd << " sent CAP LS command." << std::endl;
	// std::cout << "buffer: " << buf << std::endl;
	// if (buf.find("CAP LS 302") != std::string::npos)
	// 	buf.replace(buf.find("CAP LS 302"), 10, "");
	// if (buf.find("\n") != std::string::npos)
	// 	buf.replace(buf.find("\n"), 1, "");
	// if (buf.find("\r") != std::string::npos)
	// 	buf.replace(buf.find("\r"), 1, "");
	// // sendResponse(buf, fd);
	// std::cout << "buffer: " << buf << std::endl;
	// if (buf.find("JOIN") == 0)
	// {
	// 	sendResponse(":localhost 001 JOIN", fd);
	// 	std::cout << "Client " << fd << " sent join msg" << std::endl;
	// }
}