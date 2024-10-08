/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:10:15 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/07 15:20:31 by olcoste          ###   ########.fr       */
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
    std::cout << "Clients:" << std::endl;
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        (*it)->displayInfo();  // Access the object using the pointer
    }
    std::cout << "Port: " << port << std::endl;
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

void	Server::cmdTopic( Client& client )
{
	std::cout << "Topic arg = " << client.getNickname() << std::endl;
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

    // Correct message formatting according to IRC standards
    std::string privmsg_command = ":" + sender.getNickname() + "!" + sender.getUsername() + "@server PRIVMSG " + targetChannel + " :" + message;
    std::cout << privmsg_command << std::endl;

    // Send message to all clients in the target channel
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getCurrentChanName() == targetChannel) {
            channelFound = true;
            // Don't send the message back to the sender
            if (clients[i]->getUsername() != sender.getUsername()) {
                sendMessageToClient(clients[i]->getSocket(), privmsg_command);
            }
        }
    }

    // If no channel was found, notify the sender
    if (!channelFound) {
        std::string errorMsg = "No such channel: " + targetChannel;
        sendMessageToClient(sender.getSocket(), errorMsg);
    }
}


// void Server::cmdJoin(Client& client, const std::string& channelName) {
//     client.setCurrentChannel(channelName); // Enregistrer l'utilisateur dans le canal
//     std::cout << client.getNickname() << " has joined channel: " << channelName << std::endl;

//     // Message JOIN à envoyer à tous les utilisateurs du canal
//     std::string joinMessage = ":" + client.getNickname() + " JOIN " + channelName + "\r\n";

//     // Envoyer le message JOIN à tous les clients du canal
//     for (size_t i = 0; i < clients.size(); ++i) {
//         if (clients[i]->getCurrentChannel() == channelName) {
//             sendMessageToClient(clients[i]->getSocket(), joinMessage); // Notifier chaque client
//         }
//     }
// }

void Server::cmdJoin(Client& client, const std::string& channelName) {
    channel newChannel(channelName, false, false);
    client.addChannelClient(newChannel);
    addChannel(newChannel);
     std::vector<channel*> clientChannels = client.getChannelList();  // Assuming Client has a getChannelList() method
    std::cout << client.getNickname() << " is currently in the following channels: " << std::endl;
    
    for (std::vector<channel*>::iterator it = clientChannels.begin(); it != clientChannels.end(); ++it) {
        std::cout << "- " << (*it)->getname() << std::endl;  // Assuming channel class has a getName() method
    }
    client.setCurrentChannel(channelName);

    std::cout << client.getNickname() << " has joined channel: " << channelName << std::endl;
    std::string reply = ":" + client.getNickname() + " JOIN :" + channelName;
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getCurrentChannel() == channelName) {
            sendMessageToClient(clients[i]->getSocket(), reply); // Notifier chaque client
        }
    }
    // sendMessageToClient(client.getSocket(), reply);
    //cmdPrivMsg(client, channelName, "salut a touss");
}

void Server::sendMessageToChannel(const std::string& channel, const std::string& message, Client& sender) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getCurrentChanName() == channel) {  // Check if the client is in the target channel
            if (clients[i]->getUsername() != sender.getUsername()) {  // Avoid sending the message back to the sender
                sendMessageToClient(clients[i]->getSocket(), message);  // Send the formatted message
            }
        }
    }
}

void Server::start() {
    fd_set fds;
    int i = 0;
    int maxFD;
    int FDready;
	std::string	buf;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
	buf = "";
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
			continue ;
		}

        if (FD_ISSET(server_socket, &fds)) {
            int new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            if (new_socket < 0) {
                std::cout << "Accept failed." << std::endl;
                exit(1);
            }
            //partir de test a modifier pour recuperer les noms des differents clients !
            std::string baseName = "Name";
            std::stringstream ss;
            ss << baseName << i;
              std::string indexedName = ss.str();
            //fin test 
            clients.push_back(new Client(indexedName,indexedName,new_socket));
			RplWelcome(*clients[i]);
            i++;
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
					if( buf.find('\r') == std::string::npos 
							|| buf.find('\n') == std::string::npos )
					{
						std::cout << "\nsomething wrong i can feel it\n" << buf << std::endl;
						continue;
					}
                    // Mettre la fonction pour les messages
			//		if (clients[i]->isRegistered() == false)
			//			checkPassWord(buf, *clients[i], i);
					MessageParsing(buf, *clients[i], i);
					buf.clear();
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
