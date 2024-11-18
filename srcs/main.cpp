#include "ft_irc.hpp"

int main(int arc, char **argv)
{
	(void)argv;
	if (arc == 3)
		std::cout << "start program" << std::endl;
	else
		std::cout << "Error: please arguments in the following format: \n./ircserv <port> <password>" << std::endl;
	return 0;
}