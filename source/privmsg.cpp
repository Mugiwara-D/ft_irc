/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:16:05 by olcoste           #+#    #+#             */
/*   Updated: 2024/10/07 15:49:42 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string Server::trimPriMsg(std::string& str) {
	int i=0;

    //left trim
    while (isspace(str[i])!=0)
        i++;
    str = str.substr(i,str.length()-i);

    //right trim
    i=str.length()-1;
    while (isspace(str[i])!=0)
        i--;
    str = str.substr(0,i+1);
    return (str);
}

void	Server::cmdPrivMsgg(std::string buffer, int clientSocket) //messag  vers d autre channel
{
	//std::cout << buffer << std::endl << buffer[6] << buffer[7] << buffer[8] <<std::endl;
	
	/*retire la cmd*/
	size_t firstSpace = buffer.find(' ');
	std::string tmp = buffer.substr(firstSpace);
	std::string line = trimPriMsg(tmp);
	//std::cout << "line :" << line << ", size :" << line.size() << std::endl<< std::endl;

	if (buffer[8] == '#') {
		cmdPrivMsgServ(line,clientSocket); return ;}

	/*isole le destinataire*/
	tmp = line.substr(0, line.find(' '));
	//std::cout << "tmp :" << tmp << std::endl;
	std::string destinataire = trimPriMsg(tmp);
	//std::cout << "destinataire :" << destinataire << ", size :" << destinataire.size() << std::endl<< std::endl;

	/***********************PARSING**************************/
	/*check si le destinataire existe*/
	std::vector<Client*>::iterator itClient;
	int		destinataireSocket;
	bool	found = false;

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) { //checker si le desinataire n est pas l envoyeur
		if (clientSocket == (*it)->getSocket())
		{
			//std::cout << "TTTTTTTROUVuyER" << std::endl;
			itClient = it;
		}
		else if ((*it)->getNickname() == destinataire)
		{
			
			destinataireSocket = (*it)->getSocket();
			found = true;
			//break ;
		}
	}

	/*check si le destinataire existe*/
	if (!found) {
		std::string fullMessage = ": <serverName(a remplacer)> ERR_NOSUCHNICK 401 your/channel: " + destinataire + ": No such nick/channel.\r\n";
		if (send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0) < 0) {
			std::cerr << "Failed to send error message to client" << std::endl;
		}
	return;
	}

	//std::cout << " test name " << (*itClient)->getNickname() << std::endl;
	/*isole le message* et forme le msg*/
	tmp = line.substr(line.find(' '));
	std::string msg = (*itClient)->getNickname() + " " + tmp + "\r\n"; //ligne qui sgf fault dans certain cas (*itClient)->getNickname()
	std::cout << "msg :" << msg << ", size :" << msg.size() << std::endl;

	if (send(destinataireSocket, msg.c_str(), msg.length(), 0) < 0) {
				std::cerr << "Failed to send error message to client" << std::endl;
	}
}

void	Server::cmdPrivMsgServ(std::string line, int clientSocket)
{
	/*isole le destinataire*/
	std::string tmp = line.substr(0, line.find(' '));
	//std::cout << "tmp :" << tmp << std::endl;
	std::string destinataire = trimPriMsg(tmp) ;
	//std::cout << "destinataire :" << destinataire << ", size :" << destinataire.size() << std::endl<< std::endl;

	/*isole le message* et forme le msg*/
	tmp =  "@localhost PRIVMSG "  + destinataire + line.substr(line.find(' '));
	for (size_t i = 0; i < clients.size(); ++i) {
		if (clients[i]->getSocket() == clientSocket)
				line = ":" + clients[i]->getNickname() + "!" + clients[i]->getNickname() + tmp + "\r\n";
	}
	for (size_t i = 0; i < clients.size(); ++i) {
		if ((destinataire + "\r\n" ) == clients[i]->getCurrentChannel() && clients[i]->getSocket() != clientSocket)
			if (send(clients[i]->getSocket(), line.c_str(), line.length(), 0) < 0)
				std::cerr << "Failed to send error message to client" << std::endl;
	}
}
