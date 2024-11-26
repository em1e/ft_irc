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

		for (size_t i = 0; i < _pollFds.size(); ++i)
			close(_pollFds[i].fd);
		_pollFds.clear();

		_socket.closeSocket();
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
	pollfd serverFd = {};
	serverFd.fd = _socket.getFd();
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
	if (_pollFds[index].fd == _socket.getFd())
	{
		// should we put this in it's own function?
		// make a "createNewClient()" func or something

		// add new connection to server socket
		sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);

		int client_socket = accept(_socket.getFd(), (struct sockaddr *)&client_addr, &client_len);
		if (client_socket < 0)
		{
			perror("accept failed");
			return;
		}

		// why is this here simons?
		if (fcntl(_socket.getFd(), F_SETFL, O_NONBLOCK) < 0) // Set non-blocking mode
		{
			perror("fcntl set non-blocking failed");
			return;
		}

		pollfd clientFd = {};
		clientFd.fd = client_socket;
		clientFd.events = POLLIN;
		clientFd.revents = 0;
		_pollFds.push_back(clientFd);

		_clients.push_back(new Client(client_socket, client_addr));
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
				std::cout << "index = " << index << std::endl;
				_clients[index - 1]->setNickname(nick);
				std::cout << "index = " << index << std::endl;
				std::cout << "Nickname = " << _clients[index - 1]->getNickname() << std::endl;
				if (searchByNickname("bob") != -1)
					std::cout << "FOUND FUCKING BOB" << std::endl;
			}
			else if (buf.find("USER") == 0)
			{
				std::cout << "--------------- 5 -----------------" << std::endl;
				std::cout << "Client " << _pollFds[index].fd << " sent USER command." << std::endl;
				if (_clients[index - 1]->getNickname().empty())
				{
					std::cout << "user doesn't have a nickname, unable to welcome them!" << std::endl;
					response = ":localhost 001 Error: you need to set a nickname first!\r\n";
					return ;
				}
				response =
					" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
					"  _____   \n"
					" /     \\       (\\_/)\n"
					"/       \\     (o.o )  Welcome\n"
					"|  MAIL  |    ( :   \\  to our IRC\n"
					"|  BOX   |    (\\ /   )    server " + _clients[index - 1]->getNickname() + "!\n"
					"|________|    \n"
					" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -";
				// response += ":localhost 001 " + _clients[index - 1].getNickname() + " :Welcome to " + serverNickname + " IRC server " + _clients[index].getNickname() + "!\r\n";
				response += "\r\n";
				// _clients[index - 1]->setUsername() // set clients username and realname here
				send(_pollFds[index].fd, response.c_str(), response.length(), 0);
			}
			else if (buf.find("INVITE") == 0)
			{
				std::cout << "--------------- 6 -----------------" << std::endl;
				std::cout << "buffer before : |" << buf << "|"<< std::endl;
				buf.replace(buf.find("INVITE"), 6, "");
				std::cout << "buffer after : |" << buf << "|" << std::endl;

				std::cout << "Client " << _pollFds[index].fd << " has invited" << std::endl;
			}
			else if (buf.find("PRIVMSG") == 0)
			{
				std::cout << "--------------- 7 -----------------" << std::endl;
				buf.replace(buf.find("\r"), 1, "");
				buf.replace(buf.find("\n"), 1, "");
				std::cout << "Buffer before ':' |" << buf << "|" << std::endl;
				buf.replace(buf.find("PRIVMSG "), 8, "");
				std::string name = buf.substr(0, buf.find(" "));
				size_t pos = buf.find(":");
				if (pos != std::string::npos)
					buf = buf.substr(pos + 1);
				// std::cout << "Buffer after ':' |" << buf << "|" << std::endl;
				// std::cout << "Name: |" << name << "|" << std::endl;
				if (searchByNickname(name) != -1)
				{
					std::cout << "Name: |" << name << "|" << std::endl;
					std::cout << "Client " << _clients[index - 1]->getNickname() << " has messaged " << name << ": " << buf << std::endl;
					response = ":localhost 001 PRIVMSG " + _clients[index - 1]->getNickname() + " -> " + name + " :" + buf + "\r\n";
					send(_pollFds[index].fd, response.c_str(), response.length(), 0);
					send(_pollFds[searchByNickname(name) + 1].fd, response.c_str(), response.length(), 0);
				}
				else
				{
					std::cout << "Name: |" << name << "|" << std::endl;
					std::cout << "Client " << _clients[index - 1]->getNickname() << " has tried to message " << name << ": " << buf << std::endl;
					response = ":localhost 001 " + name + " : No such nick found\r\n";
					send(_pollFds[index].fd, response.c_str(), response.length(), 0);
				}
			}
			else if (buf.find("QUIT") == 0)
			{
				std::cout << "--------------- 8 -----------------" << std::endl;
				std::cout << "Client " << _pollFds[index].fd << " sent QUIT command." << std::endl;
				clearClient(_pollFds[index].fd);
			}
			else
			{
				std::cout << "--------------- 9 -----------------" << std::endl;
				std::cout << "UNHANDLED MESSAGE: " << buf << std::endl;
			}
		}
		else
		{
			std::cout << "--------------- 10 -----------------" << std::endl;
			std::cout << "something happened to " << _pollFds[index].fd << ", will diconnect."<< std::endl;
			clearClient(_pollFds[index].fd);
			// _pollFds.erase(_pollFds.begin() + index);
		}
	}
}

bool Server::isRunning() const { return _isRunning; }

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
