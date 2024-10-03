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

	std::size_t start = arg.find_first_not_of(" ");
	if (start == std::string::npos)
		val = arg;
	else 
	{
		start = arg.find(' ');
		val = arg.substr(start + 1);
		arg = arg.substr(0, start);
	}

	std::cout << "\ncommande MODE called with : " << arg << std::endl;
	if (arg == "+i")
		client.getCurentChan(arg).setInviteOnly(!client.getCurentChan(arg).getInviteOnly());
	else if (arg == "+t")
		client.getCurentChan(arg).setLockTopic(!client.getCurentChan(arg).getLockTopic());
	else if (arg == "+k" && !val.empty())
	{
		client.getCurentChan(arg).setKey(val);
		client.getCurentChan(arg).setLocked(true);
	}
	else if (arg == "+o")
	{
		client.getCurentChan(arg).setOps(val);
	}
	else if (arg == "+l" && isNum(val))
		client.getCurentChan(arg).setUserLim(atoi(val.c_str()));


	return true;
}
