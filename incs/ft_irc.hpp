
#ifndef FT_IRC_H
# define FT_IRC_H

#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

// we can split these inlcudes into the dedicated header files if we want

// bonus
# ifndef BONUS
#  define BONUS 0
# endif

enum struct Command {
	KICK,
	INVITE,
	TOPIC,
	MODE
};

#endif