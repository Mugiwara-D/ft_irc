#include "server.hpp"

std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');

	return str.substr(first, (last - first + 1));
}

bool	changeNickTime(std::vector<Client*>::iterator itClient)
{
	time_t	currentTime = time(NULL);

	if (difftime(currentTime, (*itClient)->getLastNicknameChange()) < 5) //mettre a 30
		return false;
	else
		(*itClient)->setLastNicknameChange(currentTime);

	return true;
}

void	Server::cmdNick(std::string buffer, int clientSocket)
{
	/*separe le nickname du reste*/
	size_t firstSpace = buffer.find(' ');
	std::string nickname = trim(buffer.substr(firstSpace));
	std::string rest = trim(nickname.substr(0, nickname.find(' ')) );
	nickname = trim(rest.substr(0, rest.find('\n') - 1));


/***********************PARSING**************************/
	/*check si le nickname est deja utiliser*/
	std::vector<Client*>::iterator itClient;
	Client& client = *clients[0];

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		std::cout << (*it)->getNickname() << "    " <<  nickname << std::endl;
		if (clientSocket == (*it)->getSocket()){
			itClient = it;
			client = **it;
		}
		if ((*it)->getNickname() == nickname)
		{
			std::string fullMessage = CLIENT_ERROR::NICKNAMEINUSE((*it)->getNickname(), nickname);
			if (send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0) < 0) {
				std::cerr << "Failed to send error message to client" << std::endl;
			}
			return ;
		}
	}
	/*check la norme IRC: 9 caract max, caract non autorise, le nick ne commence pas par un nombre*/
	for (size_t i = 0; i < (nickname.length()) ; ++i) {
		if ((nickname[0] >= '0' && nickname[0] <= '9') || (!isalnum(nickname[i]) 
						&& !(nickname[i] >= '[' && nickname[i] <= '`')
		&& nickname[i] != '-' && nickname[i] != '{' && nickname[i] != '}') || i >= 9)
			{
				std::string fullMessage = 
					CLIENT_ERROR::ERRONEUSNICKNAME(client.getNickname());
				if (send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0) < 0) {
					std::cerr << "Failed to send error message to client" << std::endl;
				}
				return ;
			}
	}

	/*changement de nom tout les X seconde*/
	if (changeNickTime(itClient) == false)
	{
		std::string fullMessage = ": <serverName(a remplacer)> ERR_NICKTOOFAST 438 Nickname: " + nickname + " you change your name too quickly.\r\n";
		if (send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0) < 0) {
			std::cerr << "Failed to send error message to client" << std::endl;
		}
		return ;
	}


/**********************EXECUTION*************************/
	//std::cout <<  " nickINT = " << nickname.size() << std::endl;
	std::cout << "Last nick " << (*itClient)->getNickname() <<  " new nick " << nickname << std::endl;
	//std::cout << "Last nick " << (*itClient)->getNickname() << std::endl;
	(*itClient)->setNickname(nickname);
}
