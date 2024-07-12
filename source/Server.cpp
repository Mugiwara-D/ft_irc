#include "../include/Server.hpp"

Server::Server() : _serverFd(-1), _running(false) {
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (_serverFd == 0){
		exit(EXIT_FAILURE);
	}

	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(PORT);

	if (bind(_serverFd, (struct sockaddr *) &_addr, sizeof(_addr)) < 0){
		close(_serverFd);
		exit(EXIT_FAILURE);
	}

	if (listen(_serverFd, 3) < 0) {
		close(_serverFd);
		exit(EXIT_FAILURE);
	}
}

/*Server::Server(const Server& src) {
    std::cout << "Copy constructor called" << std::endl;
}

Server& Server::operator=(const Server& src) {
}
*/
Server::~Server() {
	stop();
	close(_serverFd);
}

void	Server::start() {
	_running = true;
	std::cout << "Irc server started on port: " << PORT << std::endl;

	while(_running) {
		int	addrlen = sizeof(_addr);
		int	clientSocket = accept(_serverFd, (struct sockaddr *) &_addr, 
				(socklen_t *) &addrlen);
		if (clientSocket < 0) {
			std::cerr << "Accept failed" << std::endl;
			continue;
		}
		_clients.push_back(clientSocket);
		std::thread(&Server::handelClient, this, clientSocket).detach();
	}
}

void	Server::stop() {
	_running = false;
	for (int clientSocket : _clients) {
		close(clientSocket);
	}
	_clients.clear();
}

void	Server::handelClient( int clientSocket ) {
	char	buffer[1024] = {0};
	int		bytesRead;
	while((bytesRead = read(clientSocket, buffer, 1024)) > 0) {
		std::string message(buffer, bytesRead);
		std::cout << "Received: " << message << std::endl;
		broadcastMessage(message, clientSocket);
		memset(buffer, 0, sizeof(buffer));
	}
	close(clientSocket);
}

void	Server::broadcastMessage( const std::string &msg, int senderFd ) {
	for (int clientSocket : _clients) {
		if (clientSocket != senderFd)
			send(clientSocket, msg.c_str(), msg.length(), 0);
	}
}
