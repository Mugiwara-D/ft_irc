#include "server.hpp"

bool	changeNickTime(std::vector<Client*>::iterator itClient)
{
	time_t	currentTime = time(NULL);

	if (difftime(currentTime, (*itClient)->getLastNicknameChange()) < 5) //mettre a 30
		return false;
	else
		(*itClient)->setLastNicknameChange(currentTime);

	return true;
}

void	Server::cmdNick(Command_s command, Client &Clt)
{
/***********************PARSING**************************/
	std::vector<Client*>::iterator itClient;
	if (command.params.size() < 1)
	{
		sendMessageToClient(Clt.getSocket(), ERROR::NEEDMOREPARAMS(Clt.getNickname(), command.command));
		return;
	}
	std::vector<std::string>::iterator itSup = command.params.begin();
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {

		if (Clt.getSocket() == (*it)->getSocket())
			itClient = it;
		if ((*it)->getNickname() == (*itSup))
		{
			std::string fullMessage = ERROR::NICKNAMEINUSE((*it)->getNickname(), (*itSup));
			sendMessageToClient(Clt.getSocket(), fullMessage);
			return ;
		}
	}
/*check la norme IRC: 9 caract max, caract non autorise, le nick ne commence pas par un nombre*/
	for (size_t i = 0; i < ((*itSup).length()) ; ++i) {
		if (((*itSup)[0] >= '0' && (*itSup)[0] <= '9') || (!isalnum((*itSup)[i]) 
						&& !((*itSup)[i] >= '[' && (*itSup)[i] <= '`')
		&& (*itSup)[i] != '-' && (*itSup)[i] != '{' && (*itSup)[i] != '}') || i >= 9)
			{
				std::string fullMessage = 
					ERROR::ERRONEUSNICKNAME(Clt.getNickname());
				sendMessageToClient(Clt.getSocket(), fullMessage);
				return ;
			}
	}

/*changement de nom tout les X seconde*/
	if (changeNickTime(itClient) == false)
	{
		std::string fullMessage = ": <serverName(a remplacer)> ERR_NICKTOOFAST 438 Nickname: " + (*itSup) + " you change your name too quickly.";
		sendMessageToClient(Clt.getSocket(), fullMessage);
		return ;
	}

/**********************EXECUTION*************************/

 	std::string newNick = ":" + (*itClient)->getNickname() + " NICK " + (*itSup) ;

	sendMessageToClient(Clt.getSocket(), newNick);
	(*itClient)->setNickname((*itSup));
}
