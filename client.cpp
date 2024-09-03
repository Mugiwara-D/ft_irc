/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:08:37 by ablancha          #+#    #+#             */
/*   Updated: 2024/08/28 14:42:09 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "client.hpp"
#include <iostream>
#include <unistd.h>

Client::Client(const std::string &user, const std::string &nick, int socket): username(user), nickname(nick), socket(socket)
{}

Client::Client(const Client &source): username(source.username), nickname(source.nickname) 
{}

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
