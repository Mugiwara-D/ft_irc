/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:16:05 by olcoste           #+#    #+#             */
/*   Updated: 2024/10/30 14:42:13 by olcoste          ###   ########.fr       */
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

void	Server::cmdPrivMsg(Command_s command, Client &Clt)//messag  vers d autre channel
{
	/***********************PARSING**************************/
	/*check si le destinataire existe*/
	std::vector<Client*>::iterator itClient;
	int		destinataireSocket;
	bool	found = false;
	std::vector<std::string>::iterator itDest = command.params.begin();

	// if ((*itDest)[0] == '#') {
	// 	cmdPrivMsgServ(command, Clt); return ;}

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getNickname() == (*itDest))
		{
			destinataireSocket = (*it)->getSocket();
			found = true;
			break ;
		}
	}

	/*check si le destinataire existe*/
	if (!found) {
		std::string fullMessage = ": <serverName(a remplacer)> ERR_NOSUCHNICK 401 your/channel: " + Clt.getNickname() + ": No such nick/channel.\r\n";
		if (send(Clt.getSocket(), fullMessage.c_str(), fullMessage.length(), 0) < 0) {
			std::cerr << "Failed to send error message to client" << std::endl;
		}
	return;
	}

 	std::string msg = ":" + Clt.getNickname() + " PRIVMSG " + (*itDest) + " :" + command.trailing;
	sendMessageToClient(destinataireSocket, msg);
}

// void	Server::cmdPrivMsgServ(std::string line, int clientSocket)
// {
// 	/*isole le destinataire*/
// 	std::string tmp = line.substr(0, line.find(' '));
// 	//std::cout << "tmp :" << tmp << std::endl;
// 	std::string destinataire = trimPriMsg(tmp) ;
// 	//std::cout << "destinataire :" << destinataire << ", size :" << destinataire.size() << std::endl<< std::endl;

// 	/*isole le message* et forme le msg*/
// 	tmp =  "@localhost PRIVMSG "  + destinataire + line.substr(line.find(' '));
// 	for (size_t i = 0; i < clients.size(); ++i) {
// 		if (clients[i]->getSocket() == clientSocket)
// 				line = ":" + clients[i]->getNickname() + "!" + clients[i]->getNickname() + tmp + "\r\n";
// 	}
// 	for (size_t i = 0; i < clients.size(); ++i) {
// 		if ((destinataire + "\r\n" ) == clients[i]->getCurrentChannel() && clients[i]->getSocket() != clientSocket)
// 			if (send(clients[i]->getSocket(), line.c_str(), line.length(), 0) < 0)
// 				std::cerr << "Failed to send error message to client" << std::endl;
// 	}
// }
