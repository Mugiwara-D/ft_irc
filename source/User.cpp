#include "server.hpp"

std::string	Server::getStrDate()
{
	const char*	days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	const char* month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	time_t	n = time(0);
	tm*		localTime = localtime(&n);

	std::ostringstream oss;

	oss << days[localTime->tm_wday] << " " << month[localTime->tm_mon] << " " << localTime->tm_mday << " " << localTime->tm_year << " " 
	<< localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << "CET";

	return oss.str();
}

std::string	Server::getCreationDate()
{
	return creationDate;
}

void	Server::cmdUser(Command_s cmd, Client& client)
{
	client.setRealname(cmd.trailing);
	client.setUsername(cmd.params[0]);

	sendMessageToClient(client.getSocket(), RPL::WELCOME(client.getNickname(), client.getUsername(), "host"));
	sendMessageToClient(client.getSocket(), RPL::YOURHOST(client.getNickname(), "irc.example.com", "1.0"));
	sendMessageToClient(client.getSocket(), RPL::CREATED(client.getNickname(), getCreationDate()));
	sendMessageToClient(client.getSocket(), RPL::MYINFO(client.getNickname(), SERV_NAME, "1.0", " o", " o"));
	sendMessageToClient(client.getSocket(), RPL::BOUNCE(client.getNickname(), "10", "10", "10"));
	sendMessageToClient(client.getSocket(), RPL::MOTDSTART(client.getNickname(), SERV_NAME));
	sendMessageToClient(client.getSocket(), RPL::MOTD(client.getNickname(), "salut bien venue sur le serveur irc le plus sketchy de tout le 93, enjoy!!"));
	sendMessageToClient(client.getSocket(), RPL::ENDOFMOTD(client.getNickname()));
}
