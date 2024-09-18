/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:09:23 by ablancha          #+#    #+#             */
/*   Updated: 2024/09/10 15:57:39 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>
#include "client.hpp"
#include "channel.hpp"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream> 

const int	PING_INTERVAL = 60;
const int	PING_TIMEOUT = 120;

class Server {
private:
    std::string password;
    std::vector<Client*> clients;
	std::vector<channel> channels;
    int port;
    int server_socket;
    struct sockaddr_in socket_adr;

	bool	running;


public:

	Server(){}
    Server(int prt , const std::string &pwd);
    Server(const Server &other);
    ~Server();

    std::string getPassword() const;
    
    void 	start();
    void 	stop();
    void 	setPassword(const std::string &pwd);
    void 	addClient(Client* client);
    void 	removeClient(const std::string &username);
    void 	displayInfo() const;
    void 	sendMessageToClient(int client_fd, const std::string& message);

	bool	checkPassWord( std::string buffer, Client& Client, int i );
    void 	MessageParsing(char buffer[1024], Client& Client, int i);

    void	cmdNick(std::string buffer, int clienSocket);
	void	cmdMode( const std::string cmdArgs );
	void	cmdTopic( Client& client );
	void	cmdPong( Client& client );
	void	cmdPing( const std::string cmdArgs );

	void	PingPong( Client& client );
};

#endif
