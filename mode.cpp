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

bool 	Server::cmdMode( std::string arg, Client& client )
{
	std::string	val;
    std::string chan;

	std::size_t start = arg.find_first_not_of(" ");
	if (start == std::string::npos)
		val = arg;
	else 
	{
		start = arg.find(' ');
		val = arg.substr(start + 1);
		chan = arg.substr(0, start);
	}

	if (arg == "+i")
		client.getCurrentChan(chan).setInviteOnly(true);
	else if (arg == "+t")
		client.getCurrentChan(chan).setLockTopic(true);
	else if (arg == "+k" && !val.empty())
	{
		client.getCurrentChan(chan).setKey(val);
		client.getCurrentChan(chan).setLocked(true);
	}
	else if (arg == "+o")
	{
		client.getCurrentChan(chan).setOps(val);
	}
	else if (arg == "+l" && isNum(val))
		client.getCurrentChan(chan).setUserLim(atoi(val.c_str()));


	return true;
}
