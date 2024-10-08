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

bool 	Server::cmdMode( std::string arg, channel& chan )
{
	std::string	val;

	std::size_t start = arg.find_first_not_of(" ");
	if (start == std::string::npos)
		val = arg;
	else 
	{
		start = arg.find(' ');
		val = arg.substr(start + 1);
		arg = arg.substr(0, start);
	}

	if (arg == "+i")
		chan.setInviteOnly(!chan.getInviteOnly());
	else if (arg == "+t")
		chan.setLockTopic(!chan.getLockTopic());
	else if (arg == "+k" && !val.empty())
	{
		chan.setKey(val);
		chan.setLocked(true);
	}
	else if (arg == "+o")
	{
		chan.setOps(val);
	}
	else if (arg == "+l" && isNum(val))
		chan.setUserLim(atoi(val.c_str()));


	return true;
}
