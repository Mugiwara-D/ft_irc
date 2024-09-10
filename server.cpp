/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:10:15 by ablancha          #+#    #+#             */
/*   Updated: 2024/09/04 16:24:53 by ablancha         ###   ########.fr       */
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

void sendReply(int clientSocket, const std::string& reply) {
    std::string message = reply + "\r\n"; // IRC protocol requires CRLF at the end of the message
    send(clientSocket, message.c_str(), message.size(), 0);
}

void sendRPL_WELCOME(int clientSocket, const std::string& nick) {
    std::string reply = ":irc.example.com 001 " + nick + " :Welcome to the Internet Relay Network " + nick;
    sendReply(clientSocket, reply);
}

std::string get_irc_password(const std::string& command) {
    std::istringstream stream(command);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.substr(0, 4) == "PASS") {
            std::string password = line.substr(5);
            return password;
        }
    }
    return "";
}

/*
bool checkCommand( std::string str )
{
	std::set<std::string> keywrd;
	keywrd.insert("KICK");
	keywrd.insert("INVITE");
	keywrd.insert("TOPIC");
	keywrd.insert("MODE");
	keywrd.insert("JOIN");

	std::set<std::string>::iterator i;
	for (i = keywrd.begin(); i != keywrd.end(); ++i){
		if (str.find(*i) != std::string::npos)
			return true;
	}
	return false;
}
*/

void	Server::cmdMode( const std::string cmdArgs )
{
	std::cout << "MODE arg = " << cmdArgs << std::endl;
}

void	Server::cmdTopic( const std::string cmdArgs )
{
	std::cout << "Topic arg = " << cmdArgs << std::endl;
}

void	Server::cmdPing( std::string cmdArgs )
{

	std::cout << cmdArgs << "\nPONG\n" << std::endl;
}

void	Server::PingPong( Client& client )
{
	if (client.getLastPing() < std::time(0) - PING_TIMEOUT)
}

void	Server::initHandler()
{
	commandMap["KICK"] = &Server::cmdTopic;
	commandMap["TOPIC"] = &Server::cmdTopic;
	commandMap["INVITE"] = &Server::cmdTopic;
	commandMap["MODE"] = &Server::cmdMode;
	commandMap["JOIN"] = &Server::cmdTopic;
	commandMap["PRIVMSG"] = &Server::cmdTopic;
	commandMap["PING"] = &Server::cmdTopic;
}

void	Server::MessageParsing(char buffer[1024], Client& Client, int i)
{
	std::string	str = buffer;
	std::string prefix;
	(void) i;
	(void) Client;

	std::size_t start = str.find_first_not_of(" \t\n\r");
	if (start == std::string::npos)
		prefix = "";

	std::string trimstr = str.substr(start);

	std::size_t firstSpace = trimstr.find(' ');
	if (firstSpace != std::string::npos) {
			prefix = trimstr.substr(0, firstSpace);
			trimstr = trimstr.substr(firstSpace + 1);
		}
	else
		prefix = "";

	if (trimstr.empty())
		trimstr = "RINE";

	if (commandMap.empty())
		initHandler();

	std::map<std::string, HandlerCmd>::iterator itr = commandMap.find(prefix);
	if (itr != commandMap.end()) {
		HandlerCmd	handler = itr->second;
		(this->*handler)(trimstr);
	} else {
		std::cout << "\nInvalide command: " << str <<std::endl;
	}
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

        FDready = select(maxFD + 1, &fds, NULL, NULL, NULL); // check les fd prets pour la lecture
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
            i++;
            //fin test 
            clients.push_back(new Client(indexedName,indexedName,new_socket));
            sendRPL_WELCOME(new_socket, "tester");
        }
        //verifier les connexions
        displayInfo(); 
        
        for (size_t i = 0; i < clients.size(); ++i) {
            int clientFD = clients[i]->getSocket();
            if (FD_ISSET(clientFD, &fds)) {
                char buffer[1024] = {0};
                int valread = read(clientFD, buffer, 1024);
                if (valread >= 0) {
                    // Mettre la fonction pour les messages
					PingPong();
					if (clients[i]->checkPing(std::time(0), 120))
						MessageParsing(buffer, *clients[i], i);
					else
						std::cout << "\nNo bitches" << std::endl;
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
    if (send(client_fd, formattedMessage.c_str(), formattedMessage.length(), 0) < 0) {
        std::cerr << "Failed to send message to client " << client_fd << std::endl;
    }
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
