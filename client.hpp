/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:08:46 by ablancha          #+#    #+#             */
/*   Updated: 2024/09/10 16:00:20 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "channel.hpp"

class Client {
private:
    std::string username;
    std::string nickname;
    std::string Currentchannel;
    time_t lastNicknameChange;
    bool Registered;
    int socket;

public:
    Client(const std::string &user, const std::string &nick, int socket);
    Client(const Client &source);
    ~Client();


    std::string getUsername() const;

    void setUsername(const std::string &user);
    
    int getSocket() const;

    std::string getNickname() const;

    void setNickname(const std::string &nick);

    void displayInfo() const;

    bool isRegistered() const;

    void setRegistered(bool val);

    time_t getLastNicknameChange() const;
    void setLastNicknameChange(const time_t newChange);

    
};

#endif