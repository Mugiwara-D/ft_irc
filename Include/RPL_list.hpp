#pragma once
#include <string>

using std::string;
static string SERV_NAME = ":irc.example.com";

namespace RPL 
{
	inline string	WELCOME( const string& nick, const string& user, const string& host ) {
		return SERV_NAME + " 001 "+ nick +" :Welcome to the IRC Network, " + nick + "!" + user + "@" + host;
	} // Sent after the client has successfully connected and logged in to the IRC server.

	inline string	YOURHOST( const string& nick, const string& serverName, const string& version ){
		return SERV_NAME + " 002 " + nick + " :Your host is " + serverName + ", running version" + version;
	} // Describes the server the user is connected to and its version.

	inline string	CREATED( const string& nick, const string& date ) {
		return SERV_NAME + " 003 " + nick + " :This server was created " + date;
	} // Shows the date and time when the IRC server was created.

	inline string	MYINFO( const string& nick, const string& server, const string& version, const string& userMode, const string& channelMode) {
		return SERV_NAME + " 004 " + nick + " " + server + " " + version + " " + userMode + " " + channelMode;
	} // Displays server-specific information like server name, version, available user modes, and channel modes.
	
	inline string	BOUNCE( const string& nick, const string& maxChan, const string& chanLim, const string& nickLen ) {
		return SERV_NAME +" 005 "+ nick +" MAXCHANNELS="+ maxChan +" CHANLIMIT=#&:"+ chanLim +" NICKLEN="+ nickLen +" :are supported by this server";
	} // Sent to show features and limits specific to the server.

	inline string	USERCLIENT( const string& nick, const string& users, const string& invisible, const string& servers ){
		return "251 " + nick + " :There are " + users + " users and " + invisible + " invisible on " + servers + " servers";
	} // Sent after connecting, shows the number of users and servers connected.

	inline string	USEROP( const string& nick, const string& ops) {
		return "252 " + nick + " " + ops + " :IRC Operators online";
	} // Shows the number of IRC Operators currently online.

	inline string	USERUNKNOWN( const string& nick, const string& numOfUnknow ) {
		return "253  " + nick + " " + numOfUnknow + " :Unknown connections";
	} // Shows the number of unknown connections (users who are not registered/logged in yet).
	
	inline string	USERCHANNEL( const string& nick, const string& channels ) {
		return "254 " + nick + " " + channels + " :channels formed";
	} // Displays the number of channels currently open on the server.
	
	inline string	USERME( const string& nick, const string& clients, const string& servers ) {
		return "255 " + nick + " :I have " + clients + " clients and " + servers + " servers";
	} // Shows details about your connection and the total number of users on the network.

	inline string	USERHOST(const std::string& nick, const std::string& userHostInfo) {
    	return "302 " + nick + " :" + userHostInfo;
	} // Returns a list of user host information in response to a USERHOST command.
	
	inline string	WHOISUSER(const std::string& nick, const std::string& user, const std::string& host, const std::string& realName) {
    	return "311 " + nick + " " + user + " " + host + " * :" + realName;
	} // WHOIS reply showing user details (nick, user, host, real name).
	
	inline string	WHOISSERVER(const std::string& nick, const std::string& server, const std::string& serverInfo) {
    	return "312 " + nick + " " + server + " :" + serverInfo;
	} // WHOIS reply showing the server a user is on.
	
	inline string	WHOISOPERATOR(const std::string& nick) {
    	return "313 " + nick + " :is an IRC operator";
	} // Indicates the target user is an IRC operator.
	
	inline string	WHOISIDLE(const std::string& nick, const string& time){
		return "317 "+ nick +" "+ time +" :seconds idle";
	}

	inline string	ENDOFWHOIS(const std::string& nick) {
    	return "318 " + nick + " :End of WHOIS list";
	} // Marks the end of a WHOIS reply.
	
	inline string	WHOISCHANNELS(const std::string& nick, const std::string& channels) {
    	return "319 " + nick + " :" + channels;
	} // Shows the channels a user is currently on.

	inline string	NOTOPIC( const string& nick, const string& channel ) {
		return "331 " + nick + " " + channel + " :No topic is set";
	} // Indicates that a channel has no topic set.
	
	inline string	TOPIC( const string& nick, const string& channel, const string& topic ) {
		return ":" + SERV_NAME + " 332 " + nick + " " + channel + " :" + topic;
	} // Sends the current topic of a channel to the client.
	
	inline string	INVITING(const std::string& nick, const std::string& invitedNick, const std::string& channel) {
    	return "341 " + nick + " " + invitedNick + " " + channel;
	} // Sent to confirm that the user has successfully invited someone to a channel.
	
	inline string	NAMREPLY( const string& nick, const string& channel ) {
		return "353 "+ nick +" = "+ channel +" :<nick1> <nick2> <nick3>";
	} // Displays the list of users in a channel.

	inline string	ENDOFNAMES( const string& nick, const string& channel ) {
		return "366  "+ nick +" "+ channel +" :End of /NAMES list";
	} // Sent after the list of users in a channel is complete.

	inline string	MOTD(const string& nick, const string& message) {
		return SERV_NAME + " 372 "+ nick +" :- " + message;
	} // Message of the Day (MOTD) text line. Multiple lines may be sent.

	inline string	MOTDSTART(const string& nick, const string& server) {
		return SERV_NAME + " 375 "+ nick +" :- "+ server +" Message of the Day -";
	} // Indicates the start of the Message of the Day.

	inline string	ENDOFMOTD(const string& nick) {
		return SERV_NAME + " 376 "+ nick +" :End of /MOTD command.";
	} // Marks the end of the Message of the Day.

	inline string	NOTONCHANNEL(const std::string& nick, const std::string& channel) {
    	return "442 " + nick + " " + channel + " :You're not on that channel";
	} // Indicates that the user is trying to perform an action on a channel they are not in.

	inline string	CAP302(const string& nickname) {
		return SERV_NAME + " CAP " + nickname + " LS :multi-prefix";
	} // respond to CAP LS 302
	
	inline string	CAPREQ(const string& nickname) {
		return SERV_NAME + " CAP " + nickname + " ACK :multi-prefix";
	}
}

namespace ERROR{

	inline string	CHANNELMODEIS(const string& nick, const string& chan, const string& mode) {
		return ":server 324 "+ nick +" "+ chan +" "+ mode +" :Current channel modes";
	} // The server returns the current modes of the channel but does not change anything because the user either didn't provide valid modes or doesn't have permission to change them.

	inline string	NOSUCHNICK(const string& nick, const string& target) {
		return "401 "+ nick +" "+ target +" :No such nick/channel";
	} // Indicates that the target nick or channel does not exist.

	inline string	NOSUCHCHANNEL(const string& nick, const string& channel) {
		return ":server 403 "+ nick +" "+ channel +" :No such channel";
	} // This error is returned when a user tries to perform an action on a channel that does not exist

	inline string	UNKNOWNCOMMAND(const string& client, const string& command) {
		return ":server_name 421  "+ client + " " + command + " :Unknown command";
	} // the client sends a non-existing or unrecognized command.
	
	inline string	NONICKNAMEGIVEN() {
		return "431 * :No Nickname given";
	}

	inline string	ERRONEUSNICKNAME(const string& nick){
		return "432 " + nick + " norme IRC invalide";
	}

	inline string	NICKNAMEINUSE(const string& nick, const string& nickname) {
		return "433 "+ nick +" "+ nickname +" :Nickname is already in use";
	} // Signals that the requested nickname is already in use.

	inline string	USERNOTINCHANNEL(const string& nick, const string& user, const string& chan) {
		return ":server 441 "+ nick + " "+ user + " "+ chan +" :They aren't on that channel";
	} // The client is trying to perform an action on a user who is not in the specified channel, such as granting operator status to a non-existent user.
	
	inline string	NEEDMOREPARAMS(const string& nick, const string& command) {
		return ":server 461 "+ nick +" " + command + " :Not enough parameters";
	} // The client didn't provide enough parameters for the MODE command.

	inline string	KEYSET(const string& nick, const string& chan) {
		return ":server 467 "+ nick + " "+ chan +" :Channel key already set";
	} // The client attempted to set a password on a channel that already has one set, and the server rejected the request.

	inline string	UNKNOWNMODE(const string& nick, const string& mode) {
		return ":server 472 "+ nick + " "+ mode +" :is unknown mode "+ mode +" to me";
	} // he mode specified by the client is not recognized by the server, and thus the mode change cannot be applied.

	inline string	INVITEONLYCHAN(const string& nick, const string& chan) {
		return ":server 473 "+ nick +" "+ chan +" :Cannot join channel (+i)";
	} // The client is trying to join a channel that is set to invite-only, and they were not invited.

	inline string	CHANOPRIVSNEEDED(const string& nick, const string& channel) {
		return "482 "+ nick +" "+ channel +" :You're not channel operator";
	} // Sent when a user tries to perform a channel operation but lacks the necessary privileges.
	
	inline string	USERSDONTMATCH(const string& nick) {
		return ":server 502 "+ nick +" :Cannot change mode for other users";
	} // The client attempted to modify a user mode, but they do not have the required privileges.

	inline string	NOTONCHANNEL(const string& nick, const string& chn) {
		return ":server 473 " + nick + " " + chn + " :Cannot join channel (+i)";
	}

	inline string	CHANNELNOEXISTE(const string& nick, const string& chn) {
		return ":server 403 " + nick + " " + chn + " :No such channel";
	}

	inline string	USERNOEXISTE(const string& nick, const string& chn) {
		return ":server 401 " + nick + " " + chn + " :No such nick/channel";
	}

	inline string	KICKNOOPERATOR(const string& nick, const string& chn) {
		return ":server 482 " + nick + " " + chn + " :You're not channel operator";
	}
}
