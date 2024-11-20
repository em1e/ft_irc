#include "ft_irc.hpp"

// this might not be needed if we want to keep things just in main
void ft_irc(char **argv)
{
	std::cout << "start program" << std::endl;
}

int main(int arc, char **argv)
{
	if (arc == 3)
		Server server(argv[1], argv[2]);
	else
		std::cout << "Error: please arguments in the following format: \n./ircserv <port> <password>" << std::endl;
	return 0;
}