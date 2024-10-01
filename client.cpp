/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:08:37 by ablancha          #+#    #+#             */
/*   Updated: 2024/09/10 15:59:36 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "client.hpp"
#include <iostream>
#include <unistd.h>

Client::Client(const std::string &user, const std::string &nick, int socket): 
	username(user), nickname(nick), socket(socket), lastPing(std::time(0)), pinged(true)
{
    this->Registered = false;
}

Client::Client(const Client &source): username(source.username), nickname(source.nickname) 
{
    this->Registered = false;
}

Client::~Client() {
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

bool	Client::checkPing( std::time_t instTime, int pingInter )
{
	if (pinged){
		std::cout << "\nalready Pinged" << std::endl;
		return false;
	} else {
		std::cout << "\nPing sent to " << username << std::endl;
		return (instTime - lastPing) >= pingInter;
	}
}

std::time_t	Client::getLastPing(){
	return lastPing;
}

void	Client::setLastPing( std::time_t timeT){
	lastPing = timeT;
}

channel*	Client::getChan(){
	return &_chan;
}
