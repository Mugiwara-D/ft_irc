#include "server.hpp"

bool	isNum( std::string str )
{
	for (std::size_t i = 0; i < str.size(); ++i )
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}

bool 	Server::cmdMode( Command_s cmd, Client& client )
{
	size_t start = cmd.params[0].find('#');

	if (start == std::string::npos || cmd.params[0].length() <= start + 1) {
		sendMessageToClient(client.getSocket(), ERROR::NEEDMOREPARAMS(client.getNickname(), "MODE"));
		return false;
	}

	std::string	channel = cmd.params[0];

	if (!isValidChan(channel)) {
		sendMessageToClient(client.getSocket(), ERROR::NOSUCHCHANNEL(client.getNickname(), channel));
		return false;
	}
	if (cmd.params[1] == "+i" && !client.getCurrentChan(channel).getInviteOnly()) {
		client.getCurrentChan(channel).setInviteOnly(true);
	} else if (cmd.params[1] == "-i" && client.getCurrentChan(channel).getInviteOnly()) {
		client.getCurrentChan(channel).setInviteOnly(false);
	} else if (cmd.params[1] == "+t" && !client.getCurrentChan(channel).getLockTopic()) {
		client.getCurrentChan(channel).setLockTopic(true);
	} else if (cmd.params[1] == "-t" && client.getCurrentChan(channel).getLockTopic()) {
		client.getCurrentChan(channel).setLockTopic(false);
	} else if (cmd.params[1] == "+k" && !client.getCurrentChan(channel).getLock()) {
		client.getCurrentChan(channel).setKey(cmd.params[2]);
		client.getCurrentChan(channel).setLocked(true);
	} else if (cmd.params[1] == "-k" && client.getCurrentChan(channel).getLock()) {
		client.getCurrentChan(channel).setLocked(false);
	} else if (cmd.params[1] == "+o" && client.getCurrentChan(channel).isOperator(client)) {
		client.getCurrentChan(channel).addOps(client.getCurrentChan(channel).getClient(cmd.params[2]));
	} else if (cmd.params[1] == "-o" && client.getCurrentChan(channel).isOperator(client)) {
		client.getCurrentChan(channel).removeOps(client.getCurrentChan(channel).getClient(cmd.params[2]));
	} else if (cmd.params[1] == "+l" && !client.getCurrentChan(channel).isLimited()) {
		client.getCurrentChan(channel).setLimited(true);
		client.getCurrentChan(channel).setUserLim(atoi(cmd.params[2].c_str()));
	} else if (cmd.params[1] == "-l" && client.getCurrentChan(channel).isLimited()) {
		client.getCurrentChan(channel).setLimited(false);
	} else {
		sendMessageToClient(client.getSocket(), ERROR::UNKNOWNMODE(client.getNickname(), cmd.params[1]));
		return false;
	}

	return true;
}
