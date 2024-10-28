#include "server.hpp"

void	Server::cmdUser(Command_s cmd, Client& client)
{
	client.setRealname(cmd.trailing);
	client.setUsername(cmd.params[0]);
	sendMessageToClient(client.getSocket(), RPL::WELCOME(client.getNickname(), client.getUsername(), "host"));
}
