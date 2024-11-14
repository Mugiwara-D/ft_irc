/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:19:52 by olcoste           #+#    #+#             */
/*   Updated: 2024/11/14 15:53:10 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	Server::cmdInvite(Command_s command, Client &Clt)
{
	
	// std::cout << "cmd :" << command.command << " size :" << command.command.size() <<std::endl;
	// std::cout << "prefix :" << command.prefix << " size :" << command.prefix.size() <<std::endl;
	// std::cout << "channel :" << command.channel << " size :" << command.channel.size() <<std::endl;
	// std::cout << "trailing :" << command.trailing << " size :" << command.trailing.size() <<std::endl;
	// for(std::vector<std::string>::iterator it = command.params.begin(); it != command.params.end(); ++it)
	// {
	// 	std::cout << "it :" << (*it) << " size :" << (*it).size() <<std::endl;
	// }
	std::vector<std::string>::iterator it = command.params.begin();
	std::string usr = (*it);
	it++;
	std::vector<std::string>::iterator chn = it;
	try {
	Client& itSup = getClientByName(usr);

	bool existingChn = false;
	bool existingUsr = false;
	channel* chnVar = NULL;

	for (std::vector<channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		std::cout << "(*it)->getname():" << (*it)->getname() << " (*chn):" << (*chn) << std::endl;
		if (it == channels.end())
			break;
		else if ((*it)->getname() == (*chn))
		{
			chnVar = *it;
			existingChn = true;
		}
	}

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		std::cout << "(*it)->getNickname() :" << (*it)->getNickname() << " usr:" << usr << std::endl;
		if ((*it)->getNickname() == usr)
			existingUsr = true;
	}
	

    if (existingChn == false) {
		sendMessageToClient(Clt.getSocket(), ERROR::CHANNELNOEXISTE(Clt.getNickname(), (*chn)));
		return ;
    } else if (existingUsr == false) {
		sendMessageToClient(Clt.getSocket(), ERROR::USERNOEXISTE(Clt.getNickname(), (*chn)));
		return ;
	}

	std::string lineInvite = ":" + Clt.getNickname() + " INVITE " + usr + " " + (*chn);
	//std::cout << "La ligne pour inviter un user est:" << lineInvite << std::endl;
	sendMessageToClient(itSup.getSocket(), lineInvite);

	chnVar->addInvite(itSup);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		sendMessageToClient(Clt.getSocket(), ERROR::USERNOEXISTE(Clt.getNickname(), (*chn)));
	}
}

