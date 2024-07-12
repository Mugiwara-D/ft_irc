#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>

#define PORT 6667

class Server
{
	private:
		int					_serverFd;
		struct sockaddr_in	_addr;
		bool				_running;
		std::vector<int>	_clients;

		void	handelClient( int clientSocket );
		void	broadcastMessage( const std::string &msg, int senderFd );

	public:
		Server();
//		Server( const Server& src );
//		Server& operator=( const Server& src );
		~Server();

		void	start();
		void	stop();
};

#endif // SERVER_HPP
