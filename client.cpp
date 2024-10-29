/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:08:37 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/29 15:23:27 by maderuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "client.hpp"
#include <iostream>
#include <unistd.h>

Client::Client(const std::string &user, const std::string &nick, int socket): 
	username(user), nickname(nick), socket(socket), lastPing(std::time(0))
{
    this->Registered = false;
}

Client::Client(const Client &source): username(source.username), nickname(source.nickname) 
{
    this->Registered = false;
}

Client::~Client() {
    for (std::vector<channel*>::iterator it = Channel_list.begin(); it != Channel_list.end(); ++it) {
        delete *it;
    }
}

int Client::getSocket() const {
    return socket;
}

std::string Client::getUsername() const {
    return username;
}

void Client::setUsername(const std::string &user) {
    username = user;
}

std::string Client::getNickname() const {
    return nickname;
}

void Client::setNickname(const std::string &nick) {
    nickname = nick;
}

void Client::displayInfo() const {
    std::cout << "Username: " << username << std::endl;
    std::cout << "Nickname: " << nickname << std::endl;
}

bool Client::isRegistered() const{
    return(this->Registered);
}

void Client::setRegistered(bool val){
    this->Registered = val;
}

time_t	Client::getLastNicknameChange() const {
    return lastNicknameChange;
}

void	Client::setLastNicknameChange(const time_t newChange){
    lastNicknameChange = newChange;
}

bool	Client::needPing(int interval, int timeout)
{
    (void) timeout;
//	if (lastPing > std::time(0) - interval && lastPing < std::time(0) - timeout)
//		return true;
	return (std::time(0) - lastPing) >= interval ;
}

bool	Client::isTimeout(int timeout) {
	if (lastPing < std::time(0) - timeout)
		return true;
	return false;
}

std::time_t	Client::getLastPing(){
	return lastPing;
}

void	Client::setLastPing( std::time_t timeT){
	lastPing = timeT;
}

std::string Client::getCurrentChannel() const {
    return Currentchannel;
}

void Client::setCurrentChannel(const std::string &channel) {
    Currentchannel = channel;
}

void	Client::setRealname( std::string realname ){
	_realname = realname;
}

std::string	Client::getRealname() {
	return _realname;
}

void Client::addChannelClient(channel &newChannel) {
    for (size_t i = 0; i < Channel_list.size(); ++i) {
        if (Channel_list[i]->getname() == newChannel.getname()) {
            std::cout << "Client " << nickname << " is already in channel: " << newChannel.getname() << std::endl;
            return;
        }
    }
    Channel_list.push_back(new channel(newChannel));
    std::cout << "Client " << nickname << " added to channel: " << newChannel.getname() << std::endl;
}

void Client::removeChannelClient(const std::string &channelName) {
    for (size_t i = 0; i < Channel_list.size(); ++i) {
        if (Channel_list[i]->getname() == channelName) {
            Channel_list.erase(Channel_list.begin() + i);
            std::cout << "Client " << nickname << " removed from channel: " << channelName << std::endl;
            return;
        }
    }
}

std::vector<channel*> Client::getChannelList() const {
    return Channel_list; 
}

channel&    Client::getCurrentChan( std::string channelName )
{
    for (size_t it = 0; it < Channel_list.size(); ++it){
        if (Channel_list[it]->getname() == channelName) {
            return *Channel_list[it];
        }
    }
    return *Channel_list[0];
}

