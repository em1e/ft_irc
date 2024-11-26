#pragma once

#include <iostream>
#include <sys/socket.h> // socket()
#include <sys/types.h> // socket()
#include <arpa/inet.h> // inet_ntoa()
#include <fcntl.h> // fcntl()
#include <unistd.h> // close()
#include <cstring> // memset() and others
#include <unistd.h> // close()

class Socket
{
	public:
		Socket();
		~Socket();

		void create();
		void setOptions();
		void setNonBlocking();
		void bindSocket(const std::string& port);
		void startListening(int maxConnections);
		void closeSocket();
		
		int getFd() const { return _socketFd; }

	private:
		int _socketFd;
		
};
