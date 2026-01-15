# ft_irc

An IRC server implementation in C++98.

## Description

ft_irc is an Internet Relay Chat server that handles multiple clients simultaneously. It implements the IRC protocol, allowing users to connect, join channels, and communicate with each other.

## Features

- Multi-client support using poll()
- Channel management (join, leave, topic, modes)
- Private messaging
- User authentication with password
- Channel operator commands (kick, invite, mode)
- PING/PONG for connection keep-alive

## Supported Commands

- `NICK` - Set nickname
- `USER` - Set username
- `JOIN` - Join a channel
- `PRIVMSG` - Send private messages
- `MODE` - Set channel/user modes
- `TOPIC` - View/set channel topic
- `KICK` - Kick user from channel
- `INVITE` - Invite user to channel
- `PING/PONG` - Connection keep-alive

## Build

```bash
make
```

## Usage

```bash
./ft_irc <port> <password>
```

Example:
```bash
./ft_irc 6667 mypassword
```

Then connect using an IRC client (irssi, WeeChat, HexChat, etc.).

## Authors

- maderuel
- ablancha
- olcoste
