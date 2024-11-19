#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <ctime>
#include <vector> 
#include "channel.hpp"

class channel;

class Client {
private:
    std::string username;
    std::string _realname;
    std::string nickname;
    std::string Currentchannel;
    std::vector<channel*> Channel_list;
    time_t  lastNicknameChange;
    bool Registered;
    int socket;

	////////Ping Pong/////////
	std::time_t	lastPing;
    bool        awaitPing;

public:
    Client(const std::string &user, const std::string &nick, int socket);
    Client(const Client &source);
    ~Client();

    std::string getUsername() const;
	std::string	getRealname();
	void		setRealname( std::string realname );
    void setUsername(const std::string &user);
    int getSocket() const;
    std::string getNickname() const;
    void setNickname(const std::string &nick);
    void displayInfo() const;
    bool isRegistered() const;
    void setRegistered(bool val);
    time_t getLastNicknameChange() const;
    void setLastNicknameChange(const time_t newChange);

    bool	needPing(int interval, int timeout);
	bool	isTimeout(int timeout);
    std::time_t getLastPing();
    void    setLastPing( std::time_t timeT );
    bool    getAwaitPing();
    void    setAwaitPing( bool ap );

    channel&    getCurrentChan( std::string channelName );
    std::string getCurrentChannel() const;
    void setCurrentChannel(const std::string &channel);

    void addChannelClient(channel &newChannel);
    void removeChannelClient(const std::string &channelName);
    std::vector<channel*> getChannelList() const;

};

#endif
