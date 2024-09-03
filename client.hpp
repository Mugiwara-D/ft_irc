/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:08:46 by ablancha          #+#    #+#             */
/*   Updated: 2024/08/28 14:41:22 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
private:
    std::string username;
    std::string nickname;
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
};

#endif