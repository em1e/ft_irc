#include "Socket.hpp"

Socket::Socket() : _socketFd(-1) {}

Socket::~Socket() { closeSocket(); }

void Socket::create() 
{
	// hostent *server;
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

void Socket::setIP() {
    int sock;
    struct sockaddr_in serverAddr;
    struct hostent *server;
    char buffer[1024];
    std::string result;

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Get the server address
    server = gethostbyname("ifconfig.me");
    if (!server) {
        perror("Error: No such host");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Zero out server address structure
    bzero((char *)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);
    serverAddr.sin_port = htons(80); // HTTP port

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error connecting");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Send HTTP GET request
    std::string request = "GET / HTTP/1.1\r\nHost: ifconfig.me\r\nConnection: close\r\n\r\n";
    if (send(sock, request.c_str(), request.length(), 0) < 0) {
        perror("Error writing to socket");
        close(sock);
        exit(EXIT_FAILURE);
    }

    bzero(buffer, sizeof(buffer));
    while (recv(sock, buffer, sizeof(buffer) - 1, 0) > 0) {
        result += buffer;
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    // Parse the response to extract the IP
    std::string::size_type ipStart = result.find("\r\n\r\n");
    if (ipStart != std::string::npos) {
       _ip = result.substr(ipStart + 4);
    } else
		throw std::runtime_error("Error: Failed to parse IP from response");
}



