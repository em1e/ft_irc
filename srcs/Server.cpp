#include "Server.hpp"

Server::Server(){}

Server::Server(const std::string &port, const std::string &password)
		: _isRunning(false), _socket(0), _port(port), _password(password)
{
	// AF_INET: means IPv4 addresses
	// SOCK_STREAM: says we want to use a reliable connection, used for chats and file trasfer
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		// add better error handling / messages
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Mark the socket as non-blocking
    int flags = fcntl(_socket, F_GETFL, 0); // Get current flags
    if (flags < 0)
    {
        perror("fcntl get flags failed");
        close(_socket);
        exit(EXIT_FAILURE);
    }
    if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) < 0) // Set non-blocking mode
    {
        perror("fcntl set non-blocking failed");
        close(_socket);
        exit(EXIT_FAILURE);
    }

	sockaddr_in server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;  // server will accept connections from any client
	server_addr.sin_port = htons(std::stoi(port)); // set the port and convert it to the right format

	// bind() assigns a specific address (IP and port) to a socket
	if (bind(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind failed");
		close(_socket);
		exit(EXIT_FAILURE);
	}

	// indicates a readiness to accept client connection requests
	if (listen(_socket, MAX_CONNECTIONS) < 0)
	{
		perror("listen failed");
		close(_socket);
		exit(EXIT_FAILURE);
	}

	_isRunning = true;
	std::cout << "server started using port:" << port << std::endl;
}

Server::Server(Server const &){}
Server::~Server()
{
	if (_isRunning)
	{
		close(_socket);
		std::cout << "server has been shut down" << std::endl;
	}
}


Server &Server::operator=(Server const &a)
{
	if (this != &a)
	{
		_isRunning = a._isRunning;
		_socket = a._socket;
	}
	return *this;
}

void Server::run()
{
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	std::string message;

	while (_isRunning)
	{
		//accept() : waits for a client to connect. 
		//When a connection is established, it creates a new socket (referred to as client_socket) 
		//to handle the communication with the connected client.
		int client_socket = accept(_socket, (struct sockaddr *)&client_addr, &client_len);
		if (client_socket < 0)
		{
			perror("accept failed");
			continue;
		}
		std::cout << "Client connected to server" << std::endl;


		// Handle client communication (e.g., in a separate thread)
		// Close connection when done with close(client_socket);

		// std::cout << "enter message :";
		// std::getline(std::cin, message);
		// if (message == "QUIT")
		// 	exit(0);

	}
}

bool Server::isRunning() const
{
	return _isRunning;
}

/* The read(2) function is used to read data from the client socket. 
It retrieves data sent by the client, which can then be processed by the server. */


/**/