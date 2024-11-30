#include "Server.hpp"

bool Server::signal = false;

void Server::handle_signal(int sig)
{
	std::cout << "\nSignal Received!" << std::endl;
	(void)sig;
	Server::signal = true;
}

Server::Server(const std::string &port, const std::string &password)
	: _isRunning(false), _port(port), _password(password)
{
	startServer();
}

Server::~Server()
{
	if (_isRunning)
	{
		for (Client* client : _clients)
			delete client;
		_clients.clear();
		for (Channel* channel : _channels)
			delete channel;
		_channels.clear();
		_isRunning = false;
		std::cout << "Server has been shut down." << std::endl;
	}
}

void Server::startServer()
{
	try
	{
		_socket.create();
		_socket.setOptions();
		_socket.setNonBlocking();
		_socket.bindSocket(_port);
		_socket.startListening(MAX_CONNECTIONS);

		_isRunning = true;
		std::cout << "Server started using port: " << this->_port << std::endl;
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << "Error starting server: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::run()
{
	_poll.addFd(_socket.getFd());

	while (_isRunning && Server::signal == false)
	{
		int pollCount = _poll.waitPoll();
		if (pollCount <= 0)
			continue ;
		for (size_t i = 0; i < _poll.getSize(); ++i)
		{
			if (_poll.getFd(i).revents & POLLIN)
			{
				if (_poll.getFd(i).fd == _socket.getFd())
					createNewClient();
				else
					handleNewData(_poll.getFd(i).fd, i);
			}
		}
	
	}
}

void Server::clearClient(int clientFd)
{
	for (size_t i = 1; i < _clients.size(); ++i)
	{
		if (_clients[i]->getSocket() == clientFd)
		{
			if (_clients[i])
				_clients.erase(_clients.begin() + i);
			break;
		}
	}

	for (size_t i = 0; i < _poll.getSize(); ++i)
	{
		if (_poll.getFd(i).fd == clientFd)
		{
			_poll.removeFd(i);
			break;
		}
	}
	close(clientFd);
}

/*
	Loops through channels and fetchs the index of the channel
	with the same name as given argument.

	Returns the index of the channel with the same name,
	otherwise -1.
*/
int Server::getChannelIndex(std::string name)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->getName() == name)
			return i;
	}
	return -1;
}

/*
	Loops through all clients in all of the channels
	and compares the client given as an argument to them.

	Returns the index of the channel a given client is in,
	otherwise -1.
*/
int Server::isInChannel(Client *client)
{
	//maybe we need to check if the client is found in the channel
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		for (Client *tmp_client : _channels[i]->getClients())
		{
			if (tmp_client == client)
				return i;
		}
	}
	return -1;
}

/*
	Loops through all _clients in Server class and compares the
	nick given as an argument to them.

	Returns the index of the client with the same nickname,
	otherwise -1.
*/
int Server::searchByNickname(std::string nick)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getNickname() == nick)
			return i;
	}
	return -1;
}

/*
	Loops through all _clients in Server class and compares the
	username given as an argument to them.

	Returns the index of the client with the same username,
	otherwise -1.
*/
int Server::searchByUsername(std::string user)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getUsername() == user)
			return i;
	}
	return -1;
}

/*
	Returns the nickname of the client that has given clientsocket,
	otherwise empty string.
*/
std::string Server::getNickname(int fd)
{
	for (Client *client : _clients)
	{
		if (client->getSocket() == fd)
			return client->getNickname();
	}
	return "";
}

Client *Server::getClient(std::string nick)
{
	for (Client *client : _clients)
	{
		if (client->getNickname() == nick)
			return client;
	}
	return nullptr;
}

void Server::createNewClient()
{
	sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	int clientSocket = accept(_socket.getFd(), (struct sockaddr *)&clientAddr, &clientLen);
	if (clientSocket < 0)
	{	perror("Failed to accept client"); return;}

	_poll.addFd(clientSocket);
	_clients.push_back(new Client(clientSocket, clientAddr));
	std::cout << "Client: " << clientSocket << " is now connected!" << std::endl;
}

void Server::processCommand(std::string command, int fd, int index)
{
	if (command.find("NICK") == 0)
		nick(command, fd, index - 1);
	else if (command.find("USER") == 0)
		user(command, fd, index - 1);
	else if (command.find("PASS") == 0)
		pass(command, fd, index - 1);
	else if (command.find("JOIN") == 0)
		join(command, fd, index - 1);
	else if (command.find("CAP LS") != std::string::npos)
		capLs(fd, index - 1);
	else if (command.find("INVITE") == 0)
		invite(command, fd, index - 1);
	else if (command.find("KICK") == 0)
		kick(command, fd, index - 1);
	else if (command.find("PRIVMSG") == 0)
		privmsg(command, fd, index - 1);
	else if (command.find("TOPIC") == 0)
		topic(command, fd, index - 1);
	else if (command.find("MODE") == 0)
		mode(command, fd, index - 1);
	else if (command.find("QUIT") == 0)
	{
		std::cout << "--------------- QUIT -----------------" << std::endl;
		std::cout << "Client " << _clients[index - 1]->getNickname() << " sent QUIT command." << std::endl;
		clearClient(fd);
	}
	else
	{
		std::cout << "--------------- UNHANDLED MSG -----------------" << std::endl;
		std::cout << "UNHANDLED MESSAGE: " << command << std::endl;
	}
}

void Server::handleNewData(int fd, int index)
{
	char buffer[1024];
	bzero(buffer, sizeof(buffer));

	int bytes = recv(fd, buffer, sizeof(buffer), 0);

	if (bytes > 0)
	{
		std::stringstream buf(buffer);
		std::string command;
		
		while (std::getline(buf, command, '\r'))
		{
			if (!command.empty() && command.front() == '\n')
				command.erase(0, 1);
			if (!command.empty() && command.back() == '\n')
				command.pop_back();
			if (!command.empty())
			{
				std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - " << std::endl;
				std::cout << "Processing command: [" << command << "]" << std::endl;
				processCommand(command, fd, index);
			}
		}
	}
	else
	{
		std::cout << "--------------- DISCONNECT -----------------" << std::endl;
		std::cout << "Disconnecting client " << _clients[index - 1]->getNickname() << std::endl;
		clearClient(fd);
	}
}

void Server::sendResponse(std::string msg, int fd)
{
	std::string response = msg + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
}

void Server::sendError(std::string msg, int fd)
{
	std::string response = ":localhost " + msg + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
}

bool Server::validateClientRegistration(int fd, int index)
{
	if (!_clients[index]) {
		std::cerr << "Error: Client " << fd << " not found." << std::endl;
		return false;
	}
	if (!_clients[index]->getIsRegistered()) {
		sendError("451: You have not registered", fd);
		return false;
	}
	return true;
}