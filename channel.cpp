/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:07:08 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/29 15:22:21 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

channel::channel( std::string name, bool inviteOnly, bool lockTopic, bool locked, bool limited ) : 
	name(name), _inviteOnly(inviteOnly), _lockTopic(lockTopic), _locked(locked), _limited(limited){
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

bool	channel::isLimited() {
	return _limited;
}

void	channel::setLimited( bool val ) {
	_limited = val;
}

void	channel::setLockTopic( bool val ){
	_lockTopic = val;
}

bool	channel::getLock() {
	return _locked;
}

void	channel::setKey( std::string key ){
	_key = key;
}

void	channel::setTopic( std::string topic ){
	_topic = topic;
}

std::string	channel::getTopic() {
	return _topic;
}

void	channel::setLocked( bool val ){
	_locked = val;
}

void	channel::setUserLim( int val ){
	_userLimit = val;
}

Client&	channel::getClient(std::string clientNick){
	for (size_t i = 0; i < clientList.size(); ++i) {
		if (clientNick == clientList[i]->getNickname())
			return *clientList[i];
	}
	return *clientList[0];
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

void channel::addOps(Client& client) {
    operators.push_back(&client);
    std::cout << "Client " << client.getNickname() << " added to channel \"" << name << "\"." << std::endl;
}

void channel::removeOps(Client& client) {
    for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if (*it == &client) {
            operators.erase(it);
            std::cout << "Client " << client.getNickname() << " removed from channel \"" << name << "\"." << std::endl;
            break;
        }
    }
}

bool channel::isOperator(const Client& client) const {
    for (size_t i = 0; i < operators.size(); ++i) {
        if (operators[i] == &client) {
            return true;
        }
    }
    return false;
}

void channel::kickClient(Client& client) {
    for (std::vector<Client*>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
        if (*it == &client) {
            clientList.erase(it);
            std::cout << "Client " << client.getNickname() << " kicked from channel \"" << name << "\"." << std::endl;
            return;
        }
    }
    std::cout << "Client " << client.getNickname() << " is not in the channel \"" << name << "\"." << std::endl;
}

