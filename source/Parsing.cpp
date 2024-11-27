#include "server.hpp"

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

bool	Server::executeCmd(Command_s command, Client& client)
{/*
	std::cout << "\nCommand : " << command.command 
		<< "\nprefix : " << command.prefix <<
		"\ntrailing : " << command.trailing << std::endl;
	std::cout << "Params : " << std::endl;
	for (size_t i = 0; i < command.params.size(); ++i){
		std::cout << command.params[i] << std::endl; 
	}*/
	client.setLastPing(std::time(0));
	if (command.command == "CAP")
		CAPresponse(command.params[0], client);
	else if (command.command == "JOIN" && command.params.size() != 0)
        cmdJoin(client, command.params[0], command.params[1]);
	else if (command.command == "PING")
		PingRspd(client);
	else if (command.command == "PONG")
		Pong(client);
	else if (command.command == "MODE")
		cmdMode(command, client);
	else if (command.command == "NICK")
		cmdNick(command, client);
	else if (command.command == "QUIT")
		removeClient(client.getNickname());
	else if (command.command == "PRIVMSG")
		cmdPrivMsg(command, client);
	else if (command.command == "TOPIC")
		cmdTopic(command, client);
	else if (command.command == "WHOIS")
		cmdWhois(command, client);
	else if (command.command == "PASS")
	{
		if(!checkPassWord(command, client))
			return false;
	}
	else if (command.command == "USER")
		cmdUser(command, client);
	else if (command.command == "KICK")
		cmdKick(client, command.params[0], command.params[1]);
	else if (command.command == "INVITE")
		cmdInvite(command, client);
	else {
		std::cout << "\nInvalide Command" << std::endl;
		sendMessageToClient(client.getSocket(), ERROR::UNKNOWNCOMMAND(client.getUsername(), command.command));
	}
	return true;
}

void Server::MessageParsing(std::string buffer, Client& client, int i)
{
	(void)i;
    std::vector<std::string> commands;
    Command_s parsedCmd;
    size_t pos = 0;
    while ((pos = buffer.find("\r\n")) != std::string::npos) {
        commands.push_back(buffer.substr(0, pos));
        buffer.erase(0, pos + 2);
    }

    bool passFound = false;
    for (size_t i = 0; i < commands.size(); ++i) {
        parsedCmd = parseCommand(commands[i]);

        if (parsedCmd.command == "PASS") {
            passFound = true;
            if (parsedCmd.params.empty()) {
                //std::cout << "PASS command received but no password provided." << std::endl;
                sendMessageToClient(client.getSocket(), ERROR::PASSWDMISMATCH(client.getNickname(), "Missing password"));
                return;
            }
        } 
        else if (!passFound && parsedCmd.command != "CAP" && client.isRegistered() == false) {
          //  std::cout << "PASS command must be sent before other commands." << std::endl;
            sendMessageToClient(client.getSocket(), ERROR::PASSWDMISMATCH(client.getNickname(), "Password required"));
            return;
        }
        if (!executeCmd(parsedCmd, client)) {
            return;
        }
    }
}