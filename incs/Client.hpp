#pragma once

#include <vector>
#include <iostream>
#include <netinet/in.h> // sockaddr_in
#include <string>

class Channel;
class Client
{
	public:
		Client();
		Client(int client_socket, const sockaddr_in& client_addr);
		Client(Client const &a_copy);
		~Client();
		Client &operator=(Client const &a);

		int getSocket() const { return _socket; }
		int getIpAddress() const { return _IPaddress.sin_addr.s_addr; }
		std::string getNickname() const { return _nickname; }
		std::string getUsername() const { return _username; }
		bool getIsRegistered() const { return _isRegistered; }
		bool getIsAuthenticated() const { return _isAuthenticated; }

		void setSocket(int fd) { _socket = fd; }
		void setIpAddress(sockaddr_in ip) { _IPaddress = ip; }
		void setNickname(std::string nick) { _nickname = nick; }
		void setUsername(std::string user) { _username = user; }
		void setIsRegistered(bool value) { _isRegistered = value; }
		void setIsAuthenticated(bool value) { _isAuthenticated = value; }

		void sendMessage(const std::string &msg);
	private:
		std::string _nickname;
		std::string _username;
		int _socket;
		sockaddr_in _IPaddress;

		bool _isRegistered;
		bool _isAuthenticated;
};
