/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:16:05 by olcoste           #+#    #+#             */
/*   Updated: 2024/11/08 16:03:25 by olcoste          ###   ########.fr       */
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

	if ((*itDest)[0] == '#') {
		cmdPrivMsgServ(command, Clt); return ;}

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
		std::string fullMessage = ": server ERR_NOSUCHNICK 401 your/channel: " + Clt.getNickname() + ": No such nick/channel.\r\n";
		if (send(Clt.getSocket(), fullMessage.c_str(), fullMessage.length(), 0) < 0) {
			std::cerr << "Failed to send error message to client" << std::endl;
            return ;
		}
	return;
	}

 	std::string msg = ":" + Clt.getNickname() + " PRIVMSG " + (*itDest) + " :" + command.trailing;
	sendMessageToClient(destinataireSocket, msg);
}


void Server::cmdPrivMsgServ(Command_s command, Client &Clt)
{
    std::vector<std::string>::iterator itDest = command.params.begin();
    channel* chan = getChannelByName(*itDest);
    
    if (!chan) {
        std::string errorMsg = "No such channel: " + *itDest;
        sendMessageToClient(Clt.getSocket(), errorMsg);
        return;
    }
    const std::vector<Client*>& clientsInChannel = chan->getClientList();
    bool isMember = false;
    for (size_t i = 0; i < clientsInChannel.size(); ++i) {
        if (clientsInChannel[i] == &Clt) {
            isMember = true;
            break;
        }
    }
    if (!isMember) {
        std::string errorMsg = "You are not in the channel: " + *itDest;
        sendMessageToClient(Clt.getSocket(), errorMsg);
        return;
    }
    std::string line = ":" + Clt.getNickname() + " PRIVMSG " + *itDest + " :" + command.trailing;
    //std::cout << "Line ===========================:" << line << std::endl;
    for (size_t i = 0; i < clientsInChannel.size(); ++i) {
        if (clientsInChannel[i]->getNickname() != Clt.getNickname()) {
            sendMessageToClient(clientsInChannel[i]->getSocket(), line);
        }
    }
}


