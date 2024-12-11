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
		throw(std::runtime_error("Failed to set option (SO_REUSEADDR) on socket"));
		
	if (setsockopt(_socketFd, SOL_SOCKET, SO_KEEPALIVE, &optset, sizeof(optset)) == -1)
		throw(std::runtime_error("Failed to set option (SO_KEEPALIVE) on socket"));
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
	bzero(&Socket_addr, sizeof(Socket_addr));

	// Bind to IPv6 address (in6addr_any) and port
	Socket_addr.sin6_family = AF_INET6;
	Socket_addr.sin6_addr = in6addr_any;            // Accept any incoming connection (IPv6)
	Socket_addr.sin6_port = htons(std::stoi(port));  // Convert port to network byte order

	// Attempt to bind to IPv6 (this will also accept IPv4 connections)
	if (bind(_socketFd, (struct sockaddr *)&Socket_addr, sizeof(Socket_addr)) < 0)
		throw std::runtime_error("Failed to bind IPv6 socket");
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

void Socket::setIP(const std::string &hostname) {
	struct hostent *host;
	struct in_addr **addr_list;

	// Get host info by name (e.g., "localhost" or an external host)
	host = gethostbyname(hostname.c_str());
	if (host == nullptr) {
		throw std::runtime_error("Error: Could not resolve host");
	}
	// Get the list of addresses for the host
	addr_list = (struct in_addr **) host->h_addr_list;
	_ip = inet_ntoa(*addr_list[0]);
}
