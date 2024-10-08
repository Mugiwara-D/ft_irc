#include "server.hpp"

void Server::RplWelcome( Client& client ) 
{
    std::string reply = ":irc.example.com 001 ";
	sendMessageToClient(client.getSocket(), reply);
}


void	Server::whoRpl( Client& client)
{
	std::string rpl = _NAME_ + " 352 " + client.getNickname() + " * " 
		+ client.getUsername() + " localhost " + client.getNickname()
		+ " H 0 " + client.getUsername();
	//:server_name 352 <sender> <channel> <user> <host> <nick> <flags> :<hopcount> <realname>
	sendMessageToClient(client.getSocket(), rpl);
}
