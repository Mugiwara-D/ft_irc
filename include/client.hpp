#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <ctime>
#include <map>
#include <vector> 
#include "channel.hpp"

class channel;

class Client {
private:
    std::string username;
    std::string nickname;
    std::string Currentchannel;  // Current channel client is focused on
    std::vector<channel*> Channel_list;  // List of channels client is part of
    time_t lastNicknameChange;
    bool Registered;
    int socket;
	

	////////Ping Pong/////////
	std::time_t	lastPing;
	bool		pinged;

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
    bool isRegistered() const;
    void setRegistered(bool val);
    time_t getLastNicknameChange() const;
    void setLastNicknameChange(const time_t newChange);


    bool checkPing( std::time_t instTime, int pingInter );
    std::time_t getLastPing();
    void setLastPing( std::time_t timeT );
	
	std::string	getCurrentChanName();

    channel& getCurrentChan( std::string chanName );
    void setCurrentChannel(const std::string &channel);

    void addChannelClient(channel &newChannel);
    void removeChannelClient(const std::string &channelName);
    std::vector<channel*> getChannelList() const;

};

#endif
