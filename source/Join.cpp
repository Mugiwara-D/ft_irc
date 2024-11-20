/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:18:26 by ablancha          #+#    #+#             */
/*   Updated: 2024/11/07 16:18:47 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void Server::cmdJoin(Client& client, const std::string& channelName, const std::string& key) {
	if (channelName.empty()) {
		sendMessageToClient(client.getSocket(), ERROR::NEEDMOREPARAMS(client.getNickname(), "JOIN"));
		return;
	}
    channel* existingChannel = getChannelByName(channelName);

    if (existingChannel) {
        std::cout << "Le channel est:" << existingChannel->getInviteOnly() << std::endl;
        if (existingChannel->getInviteOnly() == true && existingChannel->checkInvite(&client) == false)/*check si le user est dans la liste*/ {
                std::cout << "Le client ne rejoint pas le channel" << std::endl;
                sendMessageToClient(client.getSocket(), ERROR::NOTONCHANNEL(client.getNickname(), channelName));
                return ;
        }      
        if (existingChannel->getLock() && !existingChannel->getKey().empty() && existingChannel->getKey() != key) {
            std::string errorMsg = "Wrong password" + channelName;
            sendMessageToClient(client.getSocket(), errorMsg);
            return;
        }
        if (existingChannel->getUserLimit() > 0 && existingChannel->getClientList().size() >= existingChannel->getUserLimit()) {
            std::string errorMsg = "User limit is reach";
            sendMessageToClient(client.getSocket(), errorMsg);
            return;
        }
        existingChannel->addClient(client);
        client.addChannelClient(*existingChannel);
    } 
    else {
        channel* newChannel = new channel(channelName, false, false, false, false);
        newChannel->addClient(client);
        newChannel->addOps(client);
        client.addChannelClient(*newChannel);
        addChannel(*newChannel);
        existingChannel = newChannel;
    }

    std::cout << client.getNickname() << " has joined channel: " << channelName << std::endl;
    std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " JOIN " + channelName;

    const std::vector<Client*>& clientsInChannel = existingChannel->getClientList();
    for (size_t i = 0; i < clientsInChannel.size(); ++i) {
        sendMessageToClient(clientsInChannel[i]->getSocket(), reply);
    }
}