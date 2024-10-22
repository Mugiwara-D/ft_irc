/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:10:15 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/16 15:56:14 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
/*######################fonction utiles : ###########################################*/
Server::Server(const Server &other)
    : password(other.password), clients(other.clients), port(other.port), running(false){}


std::string Server::getPassword() const {
    return password;
}

void Server::setPassword(const std::string &pwd) {
    password = pwd;
}

void Server::addClient(Client* client) {
    clients.push_back(client);
}

void Server::removeClient(const std::string &username) {
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getUsername() == username) {
            delete *it;
            clients.erase(it);
            std::cout << "Client with username \"" << username << "\" has been removed." << std::endl;
            return;
        }
    }
    std::cout << "Client with username \"" << username << "\" not found." << std::endl;
}

void Server::displayInfo() const {
    std::cout << "Password: " << password << std::endl;

    // Display clients
    std::cout << "Clients:" << std::endl;
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        (*it)->displayInfo(); // Assuming Client has a method to display its info
    }

    std::cout << "Port: " << port << std::endl;

    // Check if there are any channels
    if (channels.empty()) {
        std::cout << "No channels exist." << std::endl;
        return; // Return if there are no channels
    }

    // List users in all channels
    std::cout << "List of users in all channels:" << std::endl;
    for (size_t i = 0; i < channels.size(); ++i) {
        const channel& chan = channels[i]; // Use 'const channel&' for a const reference
        std::vector<Client*> clientsInChannel = chan.getClientList(); // Now this will work correctly

        // Print the channel name
        std::cout << "Channel: " << chan.getname() << std::endl; // Now this will work correctly

        // Check if there are users in the channel
        if (clientsInChannel.empty()) {
            std::cout << "  No users in this channel." << std::endl;
        } else {
            std::cout << "  Users:" << std::endl;
            for (size_t j = 0; j < clientsInChannel.size(); ++j) {
                std::cout << "    - " << clientsInChannel[j]->getNickname() << std::endl; // Display each client's nickname
            }
        }
    }
}




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
}

void sendReply(int clientSocket, const std::string& reply) {
    std::string message = reply + "\r\n";
    send(clientSocket, message.c_str(), message.size(), 0);
}
/*
void sendRPL_WELCOME(int clientSocket, const std::string& nick) {
    std::string reply = RPL::WELCOME();
    sendReply(clientSocket, reply);
}*/

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

void	Server::cmdPong( Client& client )
{
	sendMessageToClient(client.getSocket(), "PONG: server");
}

bool	Server::PingPong( Client& client )
{
	std::string	servMsg;

	if (client.getLastPing() < std::time(0) - PING_TIMEOUT) {
		std::cout << client.getNickname() << " Timed Out" << std::endl;
		servMsg = "ERROR :closing link: [" + client.getUsername() + 
			"] (Ping timeout: 120 seconds)";
		sendMessageToClient(client.getSocket(), servMsg);
		removeClient(client.getUsername());
		return false;
	} else if (client.getLastPing() < std::time(0) - PING_INTERVAL){
		servMsg = "PING :" + client.getUsername();
		sendMessageToClient(client.getSocket(), servMsg);
		client.setLastPing(std::time(0));
		std::cout << client.getNickname() << " ping sent" << std::endl;
    } else
		std::cout << client.getNickname() << " all good" << std::endl;
	return true;
}

bool	Server::checkPassWord( std::string buffer, Client& Client, int i )
{
	std::string message(buffer);
    //std::cout << "Message : "<< message <<  std::endl;
    if(Client.isRegistered() == false)
    {
        if(message.find("PASS") != std::string::npos)
        {
            if(get_irc_password(message) == getPassword())
            {
                Client.setRegistered(true);
            }
            else
            {
                sendMessageToClient(Client.getSocket(), 
						"Password required for connexion pas le bon mdp mon reuf"); 
                close(Client.getSocket());
                clients.erase(clients.begin() + i);
				return false;
            }
        }
        else
        {
            sendMessageToClient(Client.getSocket(), "Password required for connexion"); 
            // close(Client.getSocket());
            // clients.erase(clients.begin() + i);
        }
    }
    else if (Client.isRegistered() == true && !(message.find("NICK")))
    {
        std::cout << "NICK TROUVER" <<  std::endl;
        cmdNick(message, Client.getSocket());
    }
    else{
        std::cout << "Message : "<< buffer <<  std::endl;
        sendMessageToClient(Client.getSocket(), "Okay");
    }
	return true;
}

void Server::cmdPrivMsg(Client& sender, const std::string& targetChannel, const std::string& message) {
    bool channelFound = false;
    std::string privmsg_command = ":" + sender.getNickname() + "!" + sender.getUsername() + "@server PRIVMSG " + targetChannel + " :" + message;
    std::cout << privmsg_command << std::endl;
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getCurrentChannel() == targetChannel) {
            channelFound = true;
            if (clients[i]->getUsername() != sender.getUsername()) {
                sendMessageToClient(clients[i]->getSocket(), privmsg_command);
            }
        }
    }
    if (!channelFound) {
        std::string errorMsg = "No such channel: " + targetChannel;
        sendMessageToClient(sender.getSocket(), errorMsg);
    }
}

channel* Server::getChannelByName(const std::string& channelName) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].getname() == channelName) {
            return &channels[i];
        }
    }
    return NULL;
}

void Server::cmdKick(Client& client, const std::string& channelName)
{
    channel* chan = getChannelByName(channelName);
    if (chan == NULL) {
        std::cout << "Channel \"" << channelName << "\" does not exist." << std::endl;
        return;
    }
    if (!chan->isOperator(client)) {
        std::cout << "Client " << client.getNickname() << " is not an operator in channel \"" << channelName << "\"." << std::endl;
        return;
    }
}

void Server::cmdJoin(Client& client, const std::string& channelName) {
    channel* existingChannel = getChannelByName(channelName);

    if (existingChannel) {
        existingChannel->addClient(client);
        client.addChannelClient(*existingChannel);
    } else {

        channel newChannel(channelName, false, false);
        newChannel.addClient(client);
        newChannel.addOps(client);
        client.addChannelClient(newChannel);
        addChannel(newChannel);
    }

    std::cout << client.getNickname() << " has joined channel: " << channelName << std::endl;
    std::string reply = ":" + client.getNickname() + " JOIN :" + channelName;
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getCurrentChannel() == channelName) {
            sendMessageToClient(clients[i]->getSocket(), reply);
        }
    }
}


// void Server::cmdJoin(Client& client, const std::string& channelName) {
//     channel newChannel(channelName, false, false);
//     client.addChannelClient(newChannel);
//     addChannel(newChannel);
//     newChannel.addClient(client);


//     /* Lister les channels de client pour debug */
//     // std::vector<channel*> clientChannels = client.getChannelList();
//     // std::cout << client.getNickname() << " is currently in the following channels: " << std::endl;
    
//     // for (std::vector<channel*>::iterator it = clientChannels.begin(); it != clientChannels.end(); ++it) {
//     //     std::cout << "- " << (*it)->getname() << std::endl;
//     // }
//     // client.setCurrentChannel(channelName);



//     std::cout << client.getNickname() << " has joined channel: " << channelName << std::endl;
//     std::string reply = ":" + client.getNickname() + " JOIN :" + channelName;
//     for (size_t i = 0; i < clients.size(); ++i) {
//         if (clients[i]->getCurrentChannel() == channelName) {
//             sendMessageToClient(clients[i]->getSocket(), reply); // Notifier chaque client
//         }
//     }
//     // sendMessageToClient(client.getSocket(), reply);
//     //cmdPrivMsg(client, channelName, "salut a touss");
// }

void Server::sendMessageToChannel(const std::string& channel, const std::string& message, Client& sender) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getCurrentChannel() == channel) {
            if (clients[i]->getUsername() != sender.getUsername()) {
                sendMessageToClient(clients[i]->getSocket(), message);
            }
        }
    }
}

bool	Server::isValidChan(const std::string channel)
{
	for (size_t i = 0; i < channels.size(); ++i) {
		if (channel == channels[i].getname())
			return true;
	}

	return false;
}

void	Server::CAPresponse( std::string arg, Client& client )
{
	if (arg.find("REQ") != std::string::npos)
		sendMessageToClient(client.getSocket(), RPL::CAPREQ(client.getNickname()));
	if (arg.find("END") != std::string::npos)
		sendMessageToClient(client.getSocket(), RPL::WELCOME(client.getNickname(), client.getUsername(), "server name"));
}

bool	Server::initialHandShake( std::string buffer, int fd )
{
	if (buffer.find("CAP") != std::string::npos){
		sendMessageToClient(fd, RPL::CAP302("*"));
		return true;
	}

	return false;
}
void	rawDump(std::string msg)
{
	std::cout << "\nRaw Dump:\n"<< msg << std::endl;
}

void Server::start() {
    fd_set fds;
    int i = 0;
    int maxFD;
    int FDready;
	std::string	buf;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (running) {
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
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        FDready = select(maxFD + 1, &fds, NULL, NULL, &timeout); // check les fd prets pour la lecture
		if (FDready < 0){
			std::cout << "select failed" << std::endl;
			continue ;
		}

        if (FD_ISSET(server_socket, &fds)) {
            int new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
			if (new_socket < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK){
					continue ;
				} else {
                	std::cout << "Accept failed: " << errno << std::endl;
                	exit(1);
				}
			} 
			//partir de test a modifier pour recuperer les noms des differents clients !
			char buffer[1024] = {0};
			int bitread = read(new_socket, buffer, 1024);
			if (bitread <= 0)
				std::cout << "\nproblem" << std::endl;
			else { 
				initialHandShake(buffer, new_socket);
			}
            std::string baseName = "Guest";
            std::stringstream ss;
            ss << baseName << i;
            std::string indexedName = ss.str();
            i++;
            //fin test 
            clients.push_back(new Client(indexedName,indexedName,new_socket));
        }
        //verifier les connexions
        displayInfo(); 
        
        for (size_t i = 0; i < clients.size(); ++i) {
            int clientFD = clients[i]->getSocket();
            if (FD_ISSET(clientFD, &fds)) {
                char buffer[1024] = {0};
				if (PingPong(*clients[i]) == false)
					continue ;
                int valread = read(clientFD, buffer, 1024);
                if (valread >= 0) {
					buf += buffer;
					if(buf.find("\r\n") != std::string::npos)
					{
						if (clients[i]->isRegistered() == false)
							checkPassWord(buf, *clients[i], i);
						MessageParsing(buf, *clients[i], i);
						buf.clear();
					} else {
						removeClient(clients[i]->getUsername());
						continue ;
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
    // Envoyer un message au client
    std::string formattedMessage = message + "\r\n";
    std::cout << formattedMessage <<std::endl;
    if (send(client_fd, formattedMessage.c_str(), formattedMessage.length(), 0) < 0) {
        std::cerr << "Failed to send message to client " << client_fd << std::endl;
    }
}

void Server::addChannel(channel& newChannel) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].getname() == newChannel.getname()) {
            std::cout << "Channel " << newChannel.getname() << " already exists." << std::endl;
            return;
        }
    }
    channels.push_back(newChannel);
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
    clients.clear();
}
