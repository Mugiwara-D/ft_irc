#include "server.hpp"

void	Server::Ping(Client& client)
{
	if (client.getAwaitPing()) {
		sendMessageToClient(client.getSocket(), "PING irc.example.com");
		client.setAwaitPing(false);
	}
}

void	Server::handleTimeOut(Client& client)
{
	std::string	ErrMsg = "ERROR :closing link: [" + client.getUsername() + "] (Ping timeout: 120 seconds)";
	std::cout << client.getNickname() << " Timed Out" << std::endl;
	sendMessageToClient(client.getSocket(), ErrMsg);
	removeClient(client.getUsername());

}

bool	Server::PingRspd(Client& client) 
{

	if (client.getLastPing() < std::time(0) - PING_TIMEOUT) {
		handleTimeOut(client);
		return false;
	}
	sendMessageToClient(client.getSocket(), "PONG irc.example.com");
	client.setLastPing(std::time(0));
	client.setAwaitPing(false);
	return true;
}

bool	Server::Pong(Client& client)
{
	if (client.getLastPing() < std::time(0) - PING_TIMEOUT) {
		handleTimeOut(client);
		return false;
	} else {
		client.setLastPing(std::time(0));
	}
	return true;
}
