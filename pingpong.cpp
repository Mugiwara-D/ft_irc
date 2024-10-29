#include "server.hpp"

void	Server::Ping(Client& client)
{
		sendMessageToClient(client.getSocket(), "PING irc.example.com");
}

bool	Server::PingRspd(Client& client) 
{
	std::string	ErrMsg = "ERROR :closing link: [" + client.getUsername() + 
		"] (Ping timeout: 120 seconds)";

	if (client.getLastPing() < std::time(0) - PING_TIMEOUT) {
		std::cout << client.getNickname() << " Timed Out" << std::endl;
		sendMessageToClient(client.getSocket(), ErrMsg);
		removeClient(client.getUsername());
		return false;
	} else {
		sendMessageToClient(client.getSocket(), "PONG irc.example.com");
		client.setLastPing(std::time(0));
		std::cout << client.getNickname() << " pong sent" << std::endl;
    	}
	return true;
}

bool	Server::Pong(Client& client)
{
	std::string	ErrMsg = "ERROR :closing link: [" + client.getUsername() + 
		"] (Ping timeout: 120 seconds)";
	if (client.getLastPing() < std::time(0) - PING_TIMEOUT) {
			sendMessageToClient(client.getSocket(), ErrMsg);
			removeClient(client.getUsername());
			return false;
	} else {
		client.setLastPing(std::time(0));
	}
	return true;
}
