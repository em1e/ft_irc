#include "Server.hpp"

// Static variable
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

void Server::clearClient(int clientFd)
{
	for (size_t i = 1; i < _clients.size(); ++i)
	{
		if (_clients[i]->getSocket() == clientFd)
		{
			delete _clients[i];
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

int Server::searchByNickname(std::string nick)
{
	std::cout << "client amount = " << _clients.size() + 1 << std::endl;
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		std::cout << "client " << i << " = " << _clients[i]->getNickname() << std::endl;
		if (_clients[i]->getNickname() == nick)
			return i;
	}
	return -1;
}

void Server::createNewClient()
{
	// add new connection to server socket
	sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	int clientSocket = accept(_socket.getFd(), (struct sockaddr *)&clientAddr, &clientLen);
	if (clientSocket < 0)
	{
		perror("Failed to accept client");
		return;
	}

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
			join(response, fd);
		else if (buf.find("NICK") == 0)
			nick(buf, response, fd, index);
		else if (buf.find("USER") == 0)
			user(response, fd, index);
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