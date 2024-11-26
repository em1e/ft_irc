#include "Socket.hpp"

Socket::Socket() : _socketFd(-1) {}

Socket::~Socket() { closeSocket(); }

void Socket::create() 
{
	// AF_INET6: supports both IPv6 and IPv4 addresses
	// SOCK_STREAM: realiable connection between client and Socket
	_socketFd = socket(AF_INET6, SOCK_STREAM, 0);
	if (_socketFd < 0)
		throw std::runtime_error("Failed to create socket");
}

void Socket::setOptions()
{
	// Allow both IPv6 and IPv4 by disabling the IPV6_V6ONLY restriction
	int optset = 0;
	if (setsockopt(_socketFd, IPPROTO_IPV6, IPV6_V6ONLY, &optset, sizeof(optset)) == -1)
		throw std::runtime_error("Failed to set IPV6_V6ONLY option");

	// Allow reuse of the address to prevent "address already in use" errors during restarts
	optset = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &optset, sizeof(optset)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
}

void Socket::setNonBlocking()
{
	// Mark the socket as non-blocking
	int flags = fcntl(_socketFd, F_GETFL, 0);
	if (flags < 0)
		throw std::runtime_error("Failed to get socket flags");
	if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) < 0)
		throw std::runtime_error("Failed to set socket to non-blocking");
}

void Socket::bindSocket(const std::string& port)
{
	sockaddr_in6 Socket_addr;
	std::memset(&Socket_addr, 0, sizeof(Socket_addr));

	Socket_addr.sin6_family = AF_INET6;
	Socket_addr.sin6_addr = in6addr_any;			// Socket will accept connections from any client
	Socket_addr.sin6_port = htons(std::stoi(port));	// set the port and convert it to the right format

	// bind() assigns a specific address (IP and port) to a socket
	if (bind(_socketFd, (struct sockaddr *)&Socket_addr, sizeof(Socket_addr)) < 0)
		throw std::runtime_error("Failed to bind socket");
}

void Socket::startListening(int maxConnections)
{
	// indicates a readiness to accept client connection requests
	if (listen(_socketFd, maxConnections) < 0)
		throw std::runtime_error("Failed to listen on socket");
}

void Socket::closeSocket() {
	if (_socketFd >= 0)
		close(_socketFd);
	std::cout << "Socket has been shut down." << std::endl;
	_socketFd = -1;
}