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
		for (size_t i = 0; i < _clients.size(); ++i)
			clearClient(i);
		for (size_t i = 0; i < _pollFds.size(); ++i)
			close(_pollFds[i].fd);
		_pollFds.clear();
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
	// sockaddr_in client_addr;
	// socklen_t client_len = sizeof(client_addr);

	pollfd serverFd = {};
	serverFd.fd = _socket;
	serverFd.events = POLLIN;
	_pollFds.push_back(serverFd);

	while (_isRunning)
	{
		int pollCount = poll(_pollFds.data(), _pollFds.size(), -1);
		if (pollCount < 0)
		{
			perror("poll failed");
			continue;
		}

		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN)
				handlePollEvent(i);
		}
	}

	for (size_t i = 0; i < _pollFds.size(); ++i)
		close(_pollFds[i].fd);
	_pollFds.clear();
}

void Server::handlePollEvent(size_t index)
{
	if (_pollFds[index].fd == _socket)
	{
		// new connection to server socket
		sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);

		int client_socket = accept(_socket, (struct sockaddr *)&client_addr, &client_len);
		if (client_socket < 0)
		{
			perror("accept failed");
			return;
		}
		std::cout << "Client connected: " << client_socket << std::endl;
		_clients.push_back(Client(client_socket, client_addr));

		pollfd clientFd = {};
		clientFd.fd = client_socket;
		clientFd.events = POLLIN;
		_pollFds.push_back(clientFd);
	}
	else
	{
		char buffer[1024];
		int bytes_received = recv(_pollFds[index].fd, buffer, sizeof(buffer) - 1, 0);
		std::string msg = "A Message Flooder was here!\n";
		size_t len, bytes_sent;

		if (bytes_received > 0)
		{
			// data received
			buffer[bytes_received] = '\0';
			std::cout << "msg from client " << _pollFds[index].fd << ": " << buffer << std::endl;

			len = strlen(msg.c_str());
			bytes_sent = send(_pollFds[index].fd, msg.c_str(), len, 0);
		}
		else
		{
			// client disconnect or error
			std::cout << "Client disconnected: " << _pollFds[index].fd << std::endl;
			close(_pollFds[index].fd);
			clearClient(_pollFds[index].fd);
			_pollFds.erase(_pollFds.begin() + index);
		}
	}
}

bool Server::isRunning() const
{
	return _isRunning;
}

void Server::clearClient(int clearClient)
{
	for(size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getSocket() == clearClient)
			{_clients.erase(_clients.begin() + i); break;}
	}
}

