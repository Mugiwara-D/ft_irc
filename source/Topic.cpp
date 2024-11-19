#include "server.hpp"

void	Server::cmdTopic(Command_s cmd, Client& client)
{
	if (cmd.params.size() < 1)
	{
		sendMessageToClient(client.getSocket(), ERROR::NOSUCHCHANNEL(client.getNickname(), "*"));
		return ;
	} else if (!isValidChan(cmd.params[0])){
		sendMessageToClient(client.getSocket(), ERROR::NOSUCHCHANNEL(client.getNickname(), cmd.params[0]));
		return ;
	}

	channel*	cchan = getChannelByName(cmd.params[0]);
	
	if (cmd.trailing.empty())
	{
		if (cchan->getTopic().empty())
			sendMessageToClient(client.getSocket(), RPL::NOTOPIC(client.getNickname(), cchan->getname()));
		else {
			sendMessageToClient(client.getSocket(), RPL::TOPIC(client.getNickname(), cchan->getname(), cchan->getTopic()));
			sendMessageToClient(client.getSocket(), RPL::TOPIC(client.getNickname(), cchan->getname(), cchan->getTopic()));
		}
	} else if (cchan->getLockTopic()) {
		if (cchan->isOperator(client) == false)
			sendMessageToClient(client.getSocket(), ERROR::CHANOPRIVSNEEDED(client.getNickname(), cchan->getname()));
		else {
			std::cout << "\n la " << std::endl;
			cchan->setTopic(cmd.trailing);
			sendMessageToClient(client.getSocket(), RPL::TOPIC(client.getNickname(), cchan->getname(), cchan->getTopic()));
			sendMessageToChannel(cchan->getname(), RPL::TOPIC(client.getNickname(), cchan->getname(), cchan->getTopic()), client);
		}
	} else {
		cchan->setTopic(cmd.trailing);
		sendMessageToClient(client.getSocket(), RPL::TOPIC(client.getNickname(), cchan->getname(), cchan->getTopic()));
		sendMessageToChannel(cchan->getname(), RPL::TOPIC(client.getNickname(), cchan->getname(), cchan->getTopic()), client);
	}
}
