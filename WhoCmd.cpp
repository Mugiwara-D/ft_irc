#include "server.hpp"

void	Server::cmdWhois(Command_s cmd, Client& client)
{
	for (size_t i = 0; i < clients.size(); ++i){
		if (clients[i]->getUsername() == cmd.params[i] || clients[i]->getNickname() == cmd.params[0]){
			sendMessageToClient(client.getSocket(), RPL::WHOISUSER(client.getNickname(), client.getUsername(), "host", "realname"));
			return;
		}
	}
	sendMessageToClient(client.getSocket(), ERROR::NOSUCHNICK(client.getNickname(), cmd.params[0]));
}
