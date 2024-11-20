#include "ft_irc.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

int main(int arc, char **argv)
{
	if (arc == 3)
	{
		Server server(argv[1], argv[2]);
		server.run();
	}
	else
		std::cout << "Error: please arguments in the following format: \n./ircserv <port> <password>" << std::endl;
	return 0;
}