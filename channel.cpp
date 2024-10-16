/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:07:08 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/16 13:41:43 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

channel::channel( std::string name, bool inviteOnly, bool lockTopic ) : 
	name(name), _inviteOnly(inviteOnly), _lockTopic(lockTopic){
    std::cout << "Channel \"" << name << "\" created." << std::endl;
}

channel::~channel(){
    std::cout << "Channel \"" << name << "\" destroyed." << std::endl;
}

std::string channel::getname(){
    return(this->name);
}

bool	channel::getInviteOnly(){
	return _inviteOnly;
}

void	channel::setInviteOnly( bool val ){
	_inviteOnly = val;
}

bool	channel::getLockTopic(){
	return _lockTopic;
}

void	channel::setLockTopic( bool val ){
	_lockTopic = val;
}

void	channel::setKey( std::string key ){
	_key = key;
}

void	channel::setLocked( bool val ){
	_locked = val;
}

void	channel::setUserLim( int val ){
	_userLimit = val;
}

void	channel::setOps( std::string ops ){
	_operator = ops;
}

void channel::addClient(Client& client) {
    clientList.push_back(&client);
    std::cout << "Client " << client.getNickname() << " added to channel \"" << name << "\"." << std::endl;
}

void channel::removeClient(Client& client) {
    for (std::vector<Client*>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
        if (*it == &client) {
            clientList.erase(it);
            std::cout << "Client " << client.getNickname() << " removed from channel \"" << name << "\"." << std::endl;
            break;
        }
    }
}

std::vector<Client*> channel::getClientList() {
    return clientList;
}
std::vector<Client*> channel::getClientList() const {
    return clientList;
}
std::string channel::getname() const {
    return this->name;
}
