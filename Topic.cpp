#include "server.hpp"

void	Server::cmdTopic(Command_s cmd, Client& client)
{
	channel&	cchan = client.getCurrentChan(cmd.params[0]);
	
	if (cmd.trailing.empty())
	{
		if (cchan.getTopic().empty())
			sendMessageToClient(client.getSocket(), RPL::NOTOPIC(client.getNickname(), cchan.getname()));
		else
			sendMessageToClient(client.getSocket(), RPL::TOPIC(client.getNickname(), cchan.getname(), cmd.trailing));
	} else if (cchan.getLockTopic()) {
		if (cchan.isOperator(client) == false)
			sendMessageToClient(client.getSocket(), ERROR::CHANOPRIVSNEEDED(client.getNickname(), cchan.getname()));
		else {
			cchan.setTopic(cmd.trailing);
		sendMessageToChannel(cchan.getname(), RPL::TOPIC(client.getNickname(), cchan.getname(), cmd.trailing), client);
		}
	} else {
		cchan.setTopic(cmd.trailing);
		sendMessageToChannel(cchan.getname(), RPL::TOPIC(client.getNickname(), cchan.getname(), cmd.trailing), client);
	}
}
