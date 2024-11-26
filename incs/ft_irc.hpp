#pragma once

#include <iostream>
#include <vector> // for vectors
// #include <sys/socket.h> // socket()
// #include <sys/types.h> // socket()
#include <netinet/in.h> // sockaddr_in
// #include <fcntl.h> // fcntl()
// #include <unistd.h> // close()
// #include <arpa/inet.h> // inet_ntoa()
#include <poll.h> // poll()
#include <csignal> // signal()
#include <cstring> // memset() and others
#include <unistd.h> // close()
#include <thread>

// we can split these inlcudes into the dedicated header files if we want

// bonus
# ifndef BONUS
# define BONUS 0
# endif

#define MAX_CONNECTIONS 10

// enum struct Command {
// 	KICK,
// 	INVITE,
// 	TOPIC,
// 	MODE
// };
