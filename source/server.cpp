/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:10:15 by ablancha          #+#    #+#             */
/*   Updated: 2024/11/18 15:01:53 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(const Server &other)
    : password(other.password), clients(other.clients), port(other.port), running(false){
    }


std::string Server::getPassword() const {
    return password;
}

void Server::setPassword(const std::string &pwd) {
    password = pwd;
}

void    Server::setCreationDate(const std::string date){
    creationDate = date;
}

void Server::addClient(Client* client) {
    clients.push_back(client);
}

void Server::removeClient(const std::string &username) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNickname() == username) {
            for (std::vector<channel*>::iterator ite = channels.begin(); ite != channels.end(); ++ite)
            {
                std::string ni = (*it)->getNickname();
               (*ite)->removeClient(getClientByName(ni));
            }
            delete *it;
            clients.erase(it);
            std::cout << "Client with username \"" << username << "\" has been removed." << std::endl;
            return;
        }
    }
    std::cout << "Client with username \"" << username << "\" not found." << std::endl;
}

/*
void Server::displayInfo() const {
    // std::cout << "Password: " << password << std::endl;
    // std::cout << "Clients:" << std::endl;
    // for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
    //     (*it)->displayInfo();
    // }

    // std::cout << "Port: " << port << std::endl;

    // if (channels.empty()) {
    //     std::cout << "No channels exist." << std::endl;
    //     return;
    // }
    // std::cout << "List of users in all channels:" << std::endl;
    // for (size_t i = 0; i < channels.size(); ++i) {
    //     const channel& chan = *channels[i];
    //     std::vector<Client*> clientsInChannel = chan.getClientList();
    //     std::cout << "Channel: " << chan.getname() << std::endl;
    //     if (clientsInChannel.empty()) {
    //         std::cout << "  No users in this channel." << std::endl;
    //     } else {
    //         std::cout << "  Users:" << std::endl;
    //         for (size_t j = 0; j < clientsInChannel.size(); ++j) {
    //             std::cout << "    - " << clientsInChannel[j]->getNickname() << std::endl;
    //         }
    //     }
    // }
}*/

/*######################fonctionnement du serv : ###########################################*/
Server::Server(int port, const std::string &pwd) : password(pwd), port(port){
    server_socket = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM = TCP
    if (server_socket < 0 ) {
        std::cout << "Error with socket creation." << std::endl;
        exit(1);
    }
    socket_adr.sin_family = AF_INET; // familles adresse du socket =  IPV4
    socket_adr.sin_addr.s_addr = INADDR_ANY; // adresse ip du serv  = toutes les IPs
    socket_adr.sin_port = htons(port); // port d ecoute = port converti 
    // check association socket/port + socket en mode ecoute
    int waitlist = 10; //connexions en attentes
    if (bind(server_socket, (struct sockaddr*)&socket_adr, sizeof(socket_adr)) < 0 || listen(server_socket, waitlist) < 0) {
        std::cout << "Error during init." << std::endl;
        close(server_socket);
        exit(1);
    }
	running = true;
    std::cout << "Server initialized" << std::endl;
    setCreationDate(getStrDate());
}

void sendReply(int clientSocket, const std::string& reply) {
    std::string message = reply + "\r\n";
    send(clientSocket, message.c_str(), message.size(), 0);
}

std::string get_irc_password(const std::string& command) {
    std::istringstream stream(command);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.substr(0, 4) == "PASS") {
            std::string password = line.substr(5, line.size()-6);
            std::cout << password.size()<< std::endl;
            return password;
        }
    }
    return "";
}

bool	Server::checkPassWord( Command_s cmd, Client& Client)
{
    if(Client.isRegistered() == false)
    {
        if(cmd.command == "PASS")
        {
            if(cmd.params[0] == getPassword())
            {
                Client.setRegistered(true);
            }
            else
            {
                sendMessageToClient(Client.getSocket(), ERROR::PASSWDMISMATCH(Client.getNickname(), "Password incorrect")); 
                removeClient(Client.getUsername());
				return false;
            }
        }
        else
        {
            sendMessageToClient(Client.getSocket(), "Password required for connexion"); 
        }
    }
    else{
        sendMessageToClient(Client.getSocket(), "Okay");
    }
	return true;
}

channel* Server::getChannelByName(const std::string& channelName) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i]->getname() == channelName) {
            return channels[i];
        }
    }
    return NULL;
}

Client& Server::getClientByName(const std::string& nickname) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getNickname() == nickname) {
            return *clients[i];
        }
    }
    throw std::runtime_error("Client with nickname '" + nickname + "' not found.");
}


void Server::cmdKick(Client& client, const std::string& channelName, const std::string& target) {
    
	try {
	channel* chan = getChannelByName(channelName);
    
    if (chan == NULL) {
        std::cout << "Channel \"" << channelName << "\" does not exist." << std::endl;
		sendMessageToClient(client.getSocket(), ERROR::CHANNELNOEXISTE(client.getNickname(), channelName));
        return;
    }

	Client& cli = getClientByName(target);

    if (!chan->isOperator(client)) {
        std::cout << "Client " << client.getNickname() << " is not an operator in channel \"" << channelName << "\"." << std::endl;
        sendMessageToClient(client.getSocket(), ERROR::KICKNOOPERATOR(client.getNickname(), channelName));
		return;
    }
    chan->kickClient(cli);
    std::string kickMessage = ":" + client.getUsername() + "!admin@host KICK " + target + " :Breaking rules";
    std::cout << kickMessage << std::endl;

	sendMessageToChannel(channelName, kickMessage, client);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		sendMessageToClient(client.getSocket(), ERROR::USERNOEXISTE(client.getNickname(), channelName));
	}
}

void Server::sendMessageToChannel(const std::string& channelName, const std::string& message, const Client& sender) {
    channel* chan = getChannelByName(channelName);
    if (!chan) {
        std::cerr << "Channel " << channelName << " not found for broadcasting." << std::endl;
        return;
    }

    std::vector<Client*> clientsInChannel = chan->getClientList();

   
    for (std::vector<Client*>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it) {
        Client* client = *it;
        
        
        if (client->getNickname() != sender.getNickname()) {
            sendMessageToClient(client->getSocket(), message);
        }
    }
}

bool	Server::isValidChan(const std::string channel)
{
	for (size_t i = 0; i < channels.size(); ++i) {
		if (channel == channels[i]->getname())
			return true;
	}

	return false;
}

void	Server::CAPresponse( std::string arg, Client& client )
{
	if (arg.find("LS") != std::string::npos)
		sendMessageToClient(client.getSocket(), RPL::CAP302("*"));
	if (arg.find("REQ") != std::string::npos)
		sendMessageToClient(client.getSocket(), RPL::CAPREQ(client.getNickname()));
	if (arg.find("END") != std::string::npos)
		sendMessageToClient(client.getSocket(), RPL::WELCOME(client.getNickname(), client.getUsername(), "server name"));
}

void Server::start() {
    fd_set fds;
    int i = 0;
    int maxFD;
    int FDready;
	std::string	buf;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    std::cout << std::setfill('#') << std::setw(12) << "" << " USERS " << std::setw(12) << "" << std::endl;
    while (server_running) {
        FD_ZERO(&fds); //reset les fds
        FD_SET(server_socket, &fds); //mets le fd du socket serv dqns la liste
        maxFD = server_socket;

        for (size_t i = 0; i < clients.size(); ++i) {
            int clientFD = clients[i]->getSocket();
            if (clientFD > 0) {
                FD_SET(clientFD, &fds);
            }
            if (clientFD > maxFD) {
                maxFD = clientFD;
            }
        }

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;

        FDready = select(maxFD + 1, &fds, NULL, NULL, &timeout); // check les fd prets pour la lecture
		if (FDready < 0){
			std::cout << "select failed" << std::endl;
			continue ;
		}

        if (FD_ISSET(server_socket, &fds)) {
            int new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            char ip_str[16];
            inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
			if (new_socket < 0) {
				std::cout << "Accept failed: " << errno << std::endl;
                exit(1);
			} 
			char buffer[1024] = {0};
			int bitread = read(new_socket, buffer, 1024);
			if (bitread <= 0)
				std::cout << "\nproblem" << std::endl;
            std::string baseName = "Guest";
            std::stringstream ss;
            ss << baseName << i;
            std::string indexedName = ss.str();
            clients.push_back(new Client(indexedName,indexedName,new_socket));
            getClientByName(indexedName).setClientIp(ip_str);
            MessageParsing(buffer, getClientByName(indexedName), i);
            i++;
        }
       
        for (size_t i = 0; i < clients.size(); ++i) {
            int clientFD = clients[i]->getSocket();
            if (FD_ISSET(clientFD, &fds)) {
                char buffer[1024] = {0};
				if (clients[i]->isTimeout(PING_TIMEOUT)){
                    handleTimeOut(*clients[i]);
					continue;
				}
				if (clients[i]->needPing(PING_INTERVAL, PING_TIMEOUT) && clients[i]->getAwaitPing())
					Ping(*clients[i]);
                int valread = read(clientFD, buffer, 1024);
                if (valread >= 0) {
					buf += buffer;
					if(buf.find("\r\n") != std::string::npos) {
						MessageParsing(buf, *clients[i], i);
						buf.clear();
					}
				} else {
                    close(clientFD);
                    clients.erase(clients.begin() + i);
                }
            }
        }
    }
}

void Server::sendMessageToClient(int client_fd, const std::string& message) {
    std::string formattedMessage = message + "\r\n";
    if (send(client_fd, formattedMessage.c_str(), formattedMessage.length(), 0) < 0) {
        std::cerr << "Failed to send message to client " << client_fd << std::endl;
    }
}

void Server::addChannel(channel& newChannel) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i]->getname() == newChannel.getname()) {
            std::cout << "Channel " << newChannel.getname() << " already exists." << std::endl;
            return;
        }
    }
    channels.push_back(&newChannel);
    std::cout << "Channel " << newChannel.getname() << " added successfully." << std::endl;
}


Server::~Server() {
    stop();
}

void Server::stop() {
    close(server_socket);
    for (size_t i = 0; i < clients.size(); ++i) {
        delete clients[i];
    }
	for (size_t i = 0; i < channels.size(); ++i) {
		delete channels[i];
	}
    clients.clear();
}
