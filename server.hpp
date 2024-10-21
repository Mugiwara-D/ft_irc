/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:09:23 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/16 15:25:23 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>
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
#include "client.hpp"
#include "channel.hpp"
#include "RPL_list.hpp"
#include <cerrno>

const int	PING_INTERVAL = 60;
const int	PING_TIMEOUT = 120;

struct	Command_s{
	std::string					command;
	std::string					prefix;
	std::string					channel;
	std::vector<std::string>	params;
	std::string					trailing;
};

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
    void 	MessageParsing(std::string buffer, Client& Client, int i);

    void	cmdNick(std::string buffer, int clientSocket);
	bool	cmdMode(Command_s cmd, Client& client);
	void	cmdTopic(Command_s cmd, Client& client);
	void	cmdPong(Client& client);
	void	cmdPing(const std::string cmdArgs);
    void	cmdJoin(Client& client, const std::string& channelName);
    void	cmdKick(Client& client, const std::string& channelName);
    void	cmdPrivMsg(Client& sender, const std::string& targetChannel, const std::string& message);
    void    addChannelClient(channel &newChannel);
    void    addChannel(channel& newChannel);

	bool	isValidChan(const std::string channel);

    void	sendMessageToChannel(const std::string& channel, const std::string& message, Client& sender);
	bool	PingPong(Client& client);

    /*PRIVMSG*/
    void	cmdPrivMsgg(std::string buffer, int clientSocket);
    std::string trimPriMsg(std::string& str);
    void	cmdPrivMsgServ(std::string line, int clientSocket);

	bool	initialHandShake( std::string buffer, int fd );
	void	CAPresponse( std::string arg, Client& client );

	void	executeCmd(Command_s command, Client& client);
    channel* getChannelByName(const std::string& channelName);
};

#endif
