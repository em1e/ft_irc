
#include "Server.hpp"

int main(int arc, char **argv)
{
	if (arc == 3)
	{
		try
		{
			Server server(argv[1], argv[2]);
			std::signal(SIGINT, server.handle_signal);
			std::signal(SIGQUIT, server.handle_signal);
			server.run();
		}
		catch (std::exception &e) {
			// clear everything
			std::cerr << e.what() << std::endl;
		}

	}
	else
		std::cout << "Error: please arguments in the following format: \n./ircserv <port> <password>" << std::endl;
	return 0;
}