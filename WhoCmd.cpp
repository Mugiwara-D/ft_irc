#include "server.hpp"

void	Server::cmdWhois(Command_s cmd, Client& client)
{
	for (size_t i = 0; i < clients.size(); ++i){
		if (clients[i]->getUsername() == cmd.params[0] || clients[i]->getNickname() == cmd.params[0]){
			sendMessageToClient(client.getSocket(), RPL::WHOISUSER(client.getNickname(), client.getUsername(), "localhost", client.getRealname()));

			sendMessageToClient(client.getSocket(), RPL::WHOISCHANNELS(client.getNickname(), client.getCurrentChannel()));

	//		sendMessageToClient(client.getSocket(), RPL:: );

			sendMessageToClient(client.getSocket(), RPL::ENDOFWHOIS(client.getNickname()));

			return;
		}
	}
	sendMessageToClient(client.getSocket(), ERROR::NOSUCHNICK(client.getNickname(), cmd.params[0]));
}
