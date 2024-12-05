#pragma once

#include <iostream>
#include <sys/socket.h> // socket()
#include <sys/types.h> // socket()
#include <arpa/inet.h> // inet_ntoa()
#include <fcntl.h> // fcntl()
#include <unistd.h> // close()
#include <cstring> // memset() and others
#include <string>
#include <unistd.h> // close()
#include <arpa/inet.h> // For inet_pton and inet_ntoa
#include <netdb.h>   // For gethostbyname() and hostent

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
		std::string getIP();
	private:
		int _socketFd;
		
};
