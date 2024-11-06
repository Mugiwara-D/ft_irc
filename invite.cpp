/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:19:52 by olcoste           #+#    #+#             */
/*   Updated: 2024/11/06 16:27:03 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"



void	Server::cmdInvite(Command_s command, Client &Clt)
{
	std::cout << "cmd :" << command.command << " size :" << command.command.size() <<std::endl;
	std::cout << "prefix :" << command.prefix << " size :" << command.prefix.size() <<std::endl;
	std::cout << "channel :" << command.channel << " size :" << command.channel.size() <<std::endl;
	std::cout << "trailing :" << command.trailing << " size :" << command.trailing.size() <<std::endl;
	for(std::vector<std::string>::iterator it = command.params.begin(); it != command.params.end(); ++it)
	{
		std::cout << "it :" << (*it) << " size :" << (*it).size() <<std::endl;
	}
	std::vector<std::string>::iterator it = command.params.begin();
	std::string usr = (*it);
	it++;
	std::vector<std::string>::iterator chn = it;
	// Client* itSup = nullptr;
	// int fdCltSend;
	Client& itSup = getClientByName(usr);
	/*check si le user existe*/
	// for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
	// 	std::cout << "(*it)->getNickname().size() :" << (*it)->getNickname().size() << " (*it)->getNickname():" << (*it)->getNickname() << std::endl;
	// 	std::cout << "(*usr)).size():" << (*usr).size() << " (*usr)" << (*usr) << std::endl;
	// 	if ((*it)->getNickname() == (*usr)) {
	// 		itSup = it;
	// 		fdCltSend = (*it)->getSocket();
	// 		std::cout << "Le usr pour cmd invite a etait trouver" << std::endl;
	// 		break;
			
	// 	}
	// 	else if (it == clients.end())
	// 	{
	// 		std::cout << "Le urs pour comd invite na PAS ETAIT TROUVER" << std::endl;
	// 		return ;
	// 	}
	// }

	/*check si le CHN existe*/
	bool existingChn = false;
	channel* chnVar = NULL;

	for (std::vector<channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		std::cout << "(*it)->getname():" << (*it)->getname() << " (*chn):" << (*chn) << std::endl;
		if ((*it)->getname() == (*chn))
		{
			chnVar = *it;
			existingChn = true;
		}
	}

    if (existingChn == false) {
		//msg d erreur a mettre
		std::cout << "Le CHANNEL pour invite n existe pas" << std::endl;
		return ;
    }
	std::string lineInvite = ":" + Clt.getNickname() + " INVITE " + (usr) + " " + (*chn);
	std::cout << "La ligne pour inviter un user est:" << lineInvite << std::endl;
	sendMessageToClient(itSup.getSocket(), lineInvite);

	
	chnVar->addInvite(itSup);
	// channel* existingChannel = getChannelByName((*chn));

    // if (existingChannel) {
    //     existingChannel->addClient(itSup);
    //     itSup.addChannelClient(*existingChannel);
	// }
    
}

