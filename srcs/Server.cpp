#include "Server.hpp"

// Static variable
bool Server::signal = false;

Server::Server() {}

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
	if (setsockopt(_socket, IPPROTO_IPV6, IPV6_V6ONLY, &optset, sizeof(optset)) == -1) // Set the IPV6_V6ONLY option to 0 to allow IPv4
		throw(std::runtime_error("failed to set IPV6_V6ONLY option"));
	optset = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optset, sizeof(optset)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));

	sockaddr_in6 server_addr;
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_any;			// server will accept connections from any client
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

Server::Server(Server const &) {}

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
		if (pollCount < 0 && signal == false) // waits for an event
		{
			if (errno == EINTR) // Signal interrupted the poll; continue to retry
				continue;
			perror("poll failed");
			continue;
		}
		std::cout << "= = = = = = = = = = new round = = = = = = = = = = = =" << std::endl;
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
		// should we put this in it's own function?
		// make a "createNewClient()" func or something

		// add new connection to server socket
		sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);

		int client_socket = accept(_socket, (struct sockaddr *)&client_addr, &client_len);
		if (client_socket < 0)
		{
			perror("accept failed");
			return;
		}

		// why is this here simons?
		if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0) // Set non-blocking mode
		{
			perror("fcntl set non-blocking failed");
			return;
		}

		pollfd clientFd = {};
		clientFd.fd = client_socket;
		clientFd.events = POLLIN;
		clientFd.revents = 0;
		_pollFds.push_back(clientFd);

		// Client *user = new Client(client_socket, client_addr);
		// (*user).setSocket(client_socket);
		// (*user).setIpAddress(client_addr);
		_clients.push_back(Client(client_socket, client_addr));
		std::cout << "Client: " << client_socket << " is now connected!!" << std::endl;
	}
	else
	{
		char buffer[1024];
		bzero(buffer, sizeof(buffer));
		int bytes_received = recv(_pollFds[index].fd, buffer, sizeof(buffer) - 1, 0);

		if (bytes_received > 0)
		{
			// move each of these if statement commands below to their own functions,
			// either create a "commands.cpp" file or add them to this one, either way
			// call them Server::nick(), Server::join(), etc

			// Each command case below is numbered, so that we can easier see where they went.
			// When splitting them up, please create them in that numbered order,
			// or reorder and number the splitters again.

			// -----------------------------------------

			// someone else could be working on channels and multiple users
			// it should be pretty straightforward, just check what happens when you
			// run /channel #global on one client, and /connect #global on another
			// what about sending messages? do both see them? How do we leave channels?
			// what about sending private messages to another client?

			// -----------------------------------------

			// should we have a member list command available to see all of the users in our server?
			// what about authentication? In what way do we want to implement that?
				// -> add password. Even if a client leaves, if they added a username, nick and password
				// then their info won't get removed and they can sign back in
				// -> is saving a username and nick enough for authentication for the subject? ask around

			std::string buf(buffer);
			std::string response = ":localhost 001 A Message Flooder was here! ";
			std::cout << "Message from client " << _pollFds[index].fd << ": " << buf << std::endl;
			if (buf.find("CAP LS") != std::string::npos)
			{
				std::cout << "--------------- 1 -----------------" << std::endl;
				response += "CAP LS :\r\n";
				send(_pollFds[index].fd, response.c_str(), response.length(), 0);
				std::cout << "Client " << _pollFds[index].fd << " sent CAP LS command." << std::endl;
				std::cout << "buffer: " << buf << std::endl;
				if (buf.find("CAP LS 302") != std::string::npos)
					buf.replace(buf.find("CAP LS 302"), 10, "");
				if (buf.find("\n") != std::string::npos)
					buf.replace(buf.find("\n"), 1, "");
				if (buf.find("\r") != std::string::npos)
					buf.replace(buf.find("\r"), 1, "");
				response = ":localhost 001 A Message Flooder was here! ";
				response += buf;
				std::cout << "response: " << response;
				std::cout << "buffer: " << buf << std::endl;
				if (buf.find("JOIN") == 0)
				{
					response = ":localhost 001 A Message Flooder was here! JOIN :\r\n";
					send(_pollFds[index].fd, response.c_str(), response.length(), 0);
					std::cout << "Client " << _pollFds[index].fd << " sent join msg" << std::endl;
				}
			}
			else if (buf.find("JOIN") == 0)
			{
				std::cout << "--------------- 2 -----------------" << std::endl;
				std::cout << "Client " << _pollFds[index].fd << " sent join message" << std::endl;
				response += "JOIN :\r\n";
				send(_pollFds[index].fd, response.c_str(), response.length(), 0);
			}
			else if (buf.find("NICK") == 0)
			{
				std::cout << "--------------- 4 -----------------" << std::endl;
				std::string nick = buf.substr(5);
				nick.replace(nick.find("\r"), 1, "");
				nick.replace(nick.find("\n"), 1, "");
				std::cout << "Client " << _pollFds[index].fd << " set nickname to: " << nick << std::endl;
				response += "your nickname is now " + nick + "\r\n";
				send(_pollFds[index].fd, response.c_str(), response.length(), 0);
				_clients[index].setNickname(nick);
			}
			else if (buf.find("USER") == 0)
			{
				std::cout << "--------------- 5 -----------------" << std::endl;
				std::cout << "Client " << _pollFds[index].fd << " sent USER command." << std::endl;
				response =
					" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
					"  _____   \n"
					" /     \\       (\\_/)\n"
					"/       \\     (o.o )  Welcome\n"
					"|  MAIL  |    ( :   \\  to our IRC\n"
					"|  BOX   |    (\\ /   )    server " + _clients[index].getNickname() + "!\n"
					"|________|    \n"
					" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -";
				// response += ":localhost 001 " + _clients[index].getNickname() + " :Welcome to " + serverNickname + " IRC server " + _clients[index].getNickname() + "!\r\n";
				response += "\r\n";
				send(_pollFds[index].fd, response.c_str(), response.length(), 0);
			}
			else if (buf.find("QUIT") == 0)
			{
				std::cout << "--------------- 6 -----------------" << std::endl;
				std::cout << "Client " << _pollFds[index].fd << " sent QUIT command." << std::endl;
				clearClient(_pollFds[index].fd);
			}
			else
			{
				std::cout << "--------------- 7 -----------------" << std::endl;
				std::cout << "UNHANDLED MESSAGE: " << buf << std::endl;
			}
		}
		else
		{
			std::cout << "--------------- 8 -----------------" << std::endl;
			std::cout << "something happened to " << _pollFds[index].fd << ", will diconnect."<< std::endl;
			clearClient(_pollFds[index].fd);
			// _pollFds.erase(_pollFds.begin() + index);
		}
	}
}

bool Server::isRunning() const { return _isRunning; }

void Server::clearClient(int clientFd)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].getSocket() == clientFd)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].fd == clientFd)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}
	close(clientFd);
}
