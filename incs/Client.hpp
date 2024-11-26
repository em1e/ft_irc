#pragma once

#include <vector>
#include <netinet/in.h> // sockaddr_in

class Client
{
	public:
		Client();
		Client(int, sockaddr_in);
		Client(Client const &);
		~Client();
		Client & operator=(Client const &);

		int getSocket() const { return _socket; }
		int getIpAddress() const { return _IPaddress.sin_addr.s_addr; }
		std::string getNickname() const { return _nickname; }
		std::string getUsername() const { return _username; }

		void setSocket(int fd) { _socket = fd; }
		void setIpAddress(sockaddr_in ip) { _IPaddress = ip; }
		void setBuffer(std::string buff) { _buffer += buff; }
		void setNickname(std::string nick) { _nickname = nick; }
		void setUsername(std::string user) { _username = user; }

		// Channel actions
			// write
			// reply
			// join
			// leave
	private:
		std::string _nickname;
		std::string _username;
		std::string _buffer;
		int _socket;
		sockaddr_in _IPaddress;
};
