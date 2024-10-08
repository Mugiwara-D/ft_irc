#include "server.hpp"

void	Server::MessageParsing(std::string buffer, Client& Client, int i)
{
	std::string	str = buffer;
	std::string cmd;
	std::string chan;
	std::string flag;
	std::string	arg;
	(void) i;
	
	std::size_t start = str.find_first_not_of(" \t\n\r");
	if (start == std::string::npos){
		std::cout << "ERROR : parssing : invalide format" << std::endl;
		return;
	}

	std::cout << "\nparss step 1 : " << str << " :" << std::endl;

	std::string trimstr = str.substr(start);

	std::size_t firstSpace = trimstr.find(' ');
	std::size_t	hashTag = trimstr.find('#');
	if (hashTag == std::string::npos) {
		chan = trimstr.substr(hashTag + 1);
	}
	if (firstSpace != std::string::npos) {
			cmd = trimstr.substr(0, firstSpace);
			trimstr = trimstr.substr(firstSpace + 1);
	}
	else
		cmd = "";

	if (trimstr.empty())
		trimstr = "RINE";

	if (cmd == "MODE"){
		cmdMode(trimstr, Client);
	} else if (cmd == "PING"){
		cmdPong(Client);
	} else if (cmd == "NICK") {
		cmdNick(str, Client.getSocket());
	} else if (cmd == "QUIT") {
		removeClient(Client.getUsername());
	} else if (cmd == "JOIN") {
		std::string channelName = trimstr;
        cmdJoin(Client, channelName);
	} else if (cmd == "WHOIS") {
		whoRpl(Client);
	} else if (cmd == "PRIVMSG") {
		//sendMessageToChannel()
	}
    else {
		std::cout << "\nInvalide command: " << str <<std::endl;
	}
}
