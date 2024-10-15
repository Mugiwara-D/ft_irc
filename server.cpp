/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:10:15 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/08 15:42:53 by maderuel         ###   ########.fr       */
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
        if (clients[i]->getCurrentChannel() == targetChannel) {
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
        if (clients[i]->getCurrentChannel() == channel) {  // Check if the client is in the target channel
            if (clients[i]->getUsername() != sender.getUsername()) {  // Avoid sending the message back to the sender
                sendMessageToClient(clients[i]->getSocket(), message);  // Send the formatted message
            }
        }
    }
}

void	Server::CAPresponse( std::string arg, Client& client )
{
	if (arg.find("REQ") != std::string::npos)
		sendMessageToClient(client.getSocket(), RPL::CAPREQ(client.getNickname()));
	if (arg.find("END") != std::string::npos)
		sendMessageToClient(client.getSocket(), RPL::WELCOME(client.getNickname(), client.getUsername(), "server name"));
}

std::vector<std::string>	splitBuffer(const std::string& buffer)
{
	std::vector<std::string> commands;
	std::string::size_type	start = 0;
	std::string::size_type	end = 0;

	while ((end = buffer.find("\r\n", start)) != std::string::npos)
	{
			commands.push_back(buffer.substr(start, end - start));
			start = end + 2;
	}

	if (start < buffer.size())
		commands.push_back(buffer.substr(start));
	return commands;
}

Command_s	parseCommand( const std::string rawCmd )
{
	Command_s	command;
	size_t		pos = 0;

	if (rawCmd[pos] == ':') {
		size_t	prefixEnd = rawCmd.find(' ', pos);
		if (prefixEnd != std::string::npos){
			command.prefix = rawCmd.substr(1, prefixEnd - 1);
			pos = prefixEnd + 1;
		}
	}

	size_t	cmdEnd = rawCmd.find(' ', pos);
	if (cmdEnd != std::string::npos) {
		command.command = rawCmd.substr(pos, cmdEnd - pos);
		pos = cmdEnd + 1;
	} else {
		command.command = rawCmd.substr(pos);
		return command;
	}

	while (pos < rawCmd.size())
	{
		if (rawCmd[pos] == ':') {
			command.trailing = rawCmd.substr(pos + 1);
			break;
		}

		size_t	paramEnd = rawCmd.find(' ', pos);
		if (paramEnd != std::string::npos) {
			command.params.push_back(rawCmd.substr(pos, paramEnd - pos));
			pos = paramEnd + 1;
		} else {
			command.params.push_back(rawCmd.substr(pos));
			break;
		}
	}

	return command;
}

void	Server::executeCmd(Command_s command, Client& client)
{
	std::cout << "\nCommand : " << command.command 
		<< "\nprefix : " << command.prefix <<
		"\nparams : " << command.params[0] <<
		"\ntrailing : " << command.trailing << std::endl;
	if (command.command == "CAP")
		CAPresponse(command.params[0], client);
	else if (command.command == "JOIN")
        cmdJoin(client, command.params[0]);
	else if (command.command == "PING")
		cmdPong(client);
	else if (command.command == "MODE")
		cmdMode(command.params[0], client);
	else if (command.command == "NICK")
		std::cout << "\nNICK a refaire\n" << std::endl;
	else if (command.command == "QUIT")
		removeClient(client.getUsername());
	else if (command.command == "PRIVMSG")
		std::cout << "\nPRIVMSG a refaire\n" << std::endl;
	else {
		std::cout << "\nInvalide Command" << std::endl;
		sendMessageToClient(client.getSocket(), CLIENT_ERROR::UNKNOWNCOMMAND(client.getUsername(), command.command));
	}
}

void	Server::MessageParsing(std::string buffer, Client& Client, int i)
{
	std::vector<std::string>	rawCommands;
	Command_s	parsedCmd;

	(void) i;
	rawCommands = splitBuffer(buffer);

	for (size_t i = 0; i < rawCommands.size(); ++i)
	{
		parsedCmd = parseCommand(rawCommands[i]);
		executeCmd(parsedCmd, Client);
	}
}
/*void	Server::MessageParsing(std::string buffer, Client& Client, int i)
{
	std::string	str = buffer;
	std::string prefix;
	std::vector<std::string> commands;
	(void) i;
	
	commands = splitBuffer(buffer);
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

	if (prefix == "MODE"){
		cmdMode(trimstr, Client);
	} else if (prefix == "PING"){
		cmdPong(Client);
	} else if (prefix == "NICK") {
		cmdNick(str, Client.getSocket());
	} else if (prefix == "QUIT") {
		removeClient(Client.getUsername());
	}
    else if (prefix == "JOIN") {
		std::string channelName = trimstr;
        cmdJoin(Client, channelName);
    }
    else if (prefix == "PRIVMSG")
        cmdPrivMsgg(str, Client.getSocket());
	else if (prefix == "CAP")
		CAPresponse(trimstr, Client);
    else {
		std::cout << "\nInvalide command: " << str <<std::endl;
	}
}
*/
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
