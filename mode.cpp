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

    if (cmd.params.size() < 2)
    {
		sendMessageToClient(client.getSocket(), ERROR::UNKNOWNMODE(client.getNickname(), cmd.params[0]));
        return false
    }
	std::string	channel = cmd.params[0];

	if (!isValidChan(channel)) {
		sendMessageToClient(client.getSocket(), ERROR::NOSUCHCHANNEL(client.getNickname(), channel));
		return false;
	}
	if (cmd.params[1] == "+i" && !getChannelByName(channel)->getInviteOnly()) {
		getChannelByName(channel)->setInviteOnly(true);
	} else if (cmd.params[1] == "-i" && getChannelByName(channel)->getInviteOnly()) {
		getChannelByName(channel)->setInviteOnly(false);
	} else if (cmd.params[1] == "+t" && !getChannelByName(channel)->getLockTopic()) {
		getChannelByName(channel)->setLockTopic(true);
	} else if (cmd.params[1] == "-t" && getChannelByName(channel)->getLockTopic()) {
		getChannelByName(channel)->setLockTopic(false);
	} else if (cmd.params[1] == "+k" && !getChannelByName(channel)->getLock()) {
		getChannelByName(channel)->setKey(cmd.params[2]);
		getChannelByName(channel)->setLocked(true);
	} else if (cmd.params[1] == "-k" && getChannelByName(channel)->getLock()) {
		getChannelByName(channel)->setLocked(false);
	} else if (cmd.params[1] == "+o" && getChannelByName(channel)->isOperator(client)) {
		getChannelByName(channel)->addOps(getChannelByName(channel)->getClient(cmd.params[2]));
	} else if (cmd.params[1] == "-o" && getChannelByName(channel)->isOperator(client)) {
		getChannelByName(channel)->removeOps(getChannelByName(channel)->getClient(cmd.params[2]));
	} else if (cmd.params[1] == "+l" && !getChannelByName(channel)->isLimited()) {
		getChannelByName(channel)->setLimited(true);
		getChannelByName(channel)->setUserLim(atoi(cmd.params[2].c_str()));
	} else if (cmd.params[1] == "-l" && getChannelByName(channel)->isLimited()) {
		getChannelByName(channel)->setLimited(false);
	} else {
		sendMessageToClient(client.getSocket(), ERROR::UNKNOWNMODE(client.getNickname(), cmd.params[1]));
		return false;
	}

	return true;
}
