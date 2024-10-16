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
	(void) client;
	std::cout << "\ncmd : " << cmd.command 
		<< "\nprefix : " << cmd.prefix <<
		"\ntrailing : " << cmd.trailing << std::endl;
	std::cout << "Params : " << std::endl;
	for (size_t i = 0; i < cmd.params.size() - 1; ++i){
		std::cout << cmd.params[i] << std::endl;
	}
/*	std::string chan = cmd.prefix;
	std::string arg = cmd.params[0];
	std::string val = cmd.trailing;

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

*/
	return true;
}
