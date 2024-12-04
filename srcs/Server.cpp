#include "Server.hpp"

bool Server::signal = false;

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
			if (!_clients[index - 1]->getIsAuthenticated() && command.find("USER") == 0)
			{
				sendError("491 : You have not authenticated, try using /connect <ip> <port> <password> (nickname) to connect", fd);
				std::cout << "HERE IS WHERE I SEND IT" << std::endl;
				std::cout << "client index: |" << index - 1 << "|" << std::endl;
				std::cout << "client nick: |" << _clients[index - 1]->getNickname() << "|" << std::endl;
				std::cout << "command: |" << command << "|" << std::endl;
			}
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
		std::cout << "Disconnecting client " << _clients[index - 1]->getSocket() << ", " << _clients[index - 1]->getNickname() << std::endl;
		clearClient(fd, index - 1);
	}
}

void Server::processCommand(std::string buf, int fd, int index)
{
	std::istringstream iss(buf);
	std::string command;
	iss >> command;

	std::cout << "|" << command << "|" << std::endl;

	if (command == "NICK")
		nick(buf, fd, index - 1);
	else if (command == "USER")
		user(buf, fd, index - 1);
	else if (command == "PASS") 
		pass(buf, fd, index - 1);
	else if (command == "JOIN")
		join(buf, fd, index - 1);
	else if (command == "LIST")
		list(buf, fd);
	else if (command == "PING")
		ping(buf, fd, index - 1);
	else if (command == "INVITE")
		invite(buf, fd, index - 1);
	else if (command == "CAP")
		capLs(fd, index - 1);
	else if (command == "KICK")
		kick(buf, fd, index - 1);
	else if (command == "PRIVMSG")
		privmsg(buf, fd, index - 1);
	else if (command == "TOPIC")
		topic(buf, fd, index - 1);
	else if (command == "MODE")
		mode(buf, fd, index - 1);
	else if (command == "QUIT")
	{
		std::cout << "--------------- QUIT -----------------" << std::endl;
		std::cout << "Client " << _clients[index - 1]->getNickname() << " sent QUIT buf." << std::endl;
		clearClient(fd, index - 1);
	}
	else
	{
		std::cout << "--------------- UNHANDLED MSG -----------------" << std::endl;
		std::cout << "UNHANDLED MESSAGE: " << buf << std::endl;
	}
}

