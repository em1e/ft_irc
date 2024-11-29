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
				{
					std::cout << "CREATES A NEW FUCNING CLIENT" << std::endl;
					createNewClient();
				}
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
	std::cout << "Client: " << clientSocket << " is now connected!!" << std::endl;
}

void Server::handleNewData(int fd, int index)
{
	char buffer[1024];
	bzero(buffer, sizeof(buffer));

	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes > 0)
	{
		std::string buf(buffer);
		std::string response = ":localhost 001 A Message Flooder was here! ";
		std::cout << "Message from client " << fd << ": " << buf << std::endl;
		
		if (buf.find("CAP LS") != std::string::npos)
			capLs(buf, response, fd);
		else if (buf.find("JOIN") == 0)
			join(buf, fd);
		else if (buf.find("NICK") == 0)
			nick(buf, fd, index);
		else if (buf.find("USER") == 0)
			user(buf, fd, index);
		else if (buf.find("INVITE") == 0)
			invite(buf, fd);
		else if (buf.find("PRIVMSG") == 0)
			privmsg(buf, response, fd, index);
		else if (buf.find("QUIT") == 0)
		{
			std::cout << "--------------- QUIT -----------------" << std::endl;
			std::cout << "Client " << fd << " sent QUIT command." << std::endl;
			clearClient(fd);
		}
		else
		{
			std::cout << "--------------- UNHANDLED MSG -----------------" << std::endl;
			std::cout << "UNHANDLED MESSAGE: " << buf << std::endl;
		}
	}
	else
	{
		std::cout << "--------------- WRONG -----------------" << std::endl;
		std::cout << "something happened to " << fd << ", will diconnect."<< std::endl;
		clearClient(fd);
	}
}

void Server::sendResponse(std::string msg, int fd)
{
	std::string response = ":localhost 001 A Message Flooder was here!";
	response += " " + msg + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
}