#include "Server.hpp"

// Static variable
bool Server::signal = false;

Server::Server(){}

void Server::handle_signal(int sig)
{
	std::cout << "\nSignal Received!" << std::endl;
	(void)sig;
	Server::signal = true;
}

Server::Server(const std::string &port, const std::string &password)
		: _isRunning(false), _socket(0), _port(port), _password(password)
{
	// AF_INET: means IPv4 addresses
	// AF_INET6: supports both IPv6 and IPv4 addresses
	// SOCK_STREAM: says we want to use a reliable connection, used for chats and file trasfer
	_socket = socket(AF_INET6, SOCK_STREAM, 0);
	if (_socket < 0)
	{
		// add better error handling / messages
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0) // Set non-blocking mode
	{
		perror("fcntl set non-blocking failed");
		close(_socket);
		exit(EXIT_FAILURE);
	}

	int optset = 0;
	if(setsockopt(_socket, IPPROTO_IPV6, IPV6_V6ONLY, &optset, sizeof(optset)) == -1) // Set the IPV6_V6ONLY option to 0 to allow IPv4
		throw(std::runtime_error("failed to set IPV6_V6ONLY option"));
	optset = 1;
	if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optset, sizeof(optset)) == -1)
        	throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	
	sockaddr_in6 server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_any;  // server will accept connections from any client
	server_addr.sin6_port = htons(std::stoi(port)); // set the port and convert it to the right format
	
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
	std::cout << "Server started using port: " << port << std::endl;
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
		std::cout << "Server has been shut down." << std::endl;
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
	serverFd.revents = 0;
	_pollFds.push_back(serverFd);

	while (_isRunning && Server::signal == false)
	{
		int pollCount = poll(_pollFds.data(), _pollFds.size(), -1);
		if (pollCount < 0 && signal == false) //waits for an event
		{
			if (errno == EINTR) // Signal interrupted the poll; continue to retry
            	continue; 
			perror("poll failed");
			continue;
		}

		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN)
				handlePollEvent(i); //here
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
		if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0) // Set non-blocking mode
		{
			perror("fcntl set non-blocking failed");
			// close(_socket);
			// exit(EXIT_FAILURE);
			return;
		}
		Client *user = new Client(client_socket, client_addr);
		pollfd clientFd = {};
		clientFd.fd = client_socket;
		clientFd.events = POLLIN;
		clientFd.revents = 0;
		(*user).setSocket(client_socket);
		(*user).setIpAddress(client_addr);
		_clients.push_back(*user);
		_pollFds.push_back(clientFd);
		std::cout << "Client: " << client_socket << " is now connected!!" << std::endl;
	}
	else
	{
		char buffer[1024];
		bzero(buffer, sizeof(buffer));
		int bytes_received = recv(_pollFds[index].fd, buffer, sizeof(buffer) - 1, 0);
		std::string msg = "A Message Flooder was here! ";
		size_t len, bytes_sent;

		if (bytes_received > 0)
		{
			// data received
			buffer[bytes_received] = '\0';
			std::cout << "msg from client " << _pollFds[index].fd << ": " << buffer << std::endl;
			std::cout << "Buffer: " << buffer << std::endl;
			msg += buffer;
			if (msg.find("\n") != std::string::npos)
				msg.replace(msg.find("\n"), 1, "");
			if (msg.find("\r") != std::string::npos)
				msg.replace(msg.find("\r"), 1, "");
			//msg += "\r\n";
			msg += "\n";
			len = strlen(msg.c_str());
			bytes_sent = send(_pollFds[index].fd, msg.c_str(), len, 0);
			std::cout << "bytes sent to client : " << bytes_sent << std::endl;
		}
		else
		{
			// client disconnect or error
			std::cout << "Client disconnected: " << _pollFds[index].fd << std::endl;
			close(_pollFds[index].fd);
			clearClient(_pollFds[index].fd);
			// _pollFds.erase(_pollFds.begin() + index); 
		}
	}
}


bool Server::isRunning() const
{
	return _isRunning;
}

void Server::clearClient(int clearClient)
{
	for(size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].getSocket() == clearClient)
			{_clients.erase(_clients.begin() + i); break;}
	}
	for (size_t i = 0; i < _pollFds.size(); ++i) {
        if (_pollFds[i].fd == clearClient) {
            _pollFds.erase(_pollFds.begin() + i);
            break;
        }
    }
    close(clearClient);
    std::cout << "Client disconnected: " << clearClient << std::endl;
}

