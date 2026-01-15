# ft_irc

Un serveur IRC en C++98.

## Description

ft_irc est un serveur Internet Relay Chat capable de gerer plusieurs clients simultanement. Il implemente le protocole IRC, permettant aux utilisateurs de se connecter, rejoindre des canaux et communiquer entre eux.

## Fonctionnalites

- Support multi-client avec poll()
- Gestion des canaux (rejoindre, quitter, topic, modes)
- Messages prives
- Authentification par mot de passe
- Commandes operateur (kick, invite, mode)
- PING/PONG pour maintenir la connexion

## Commandes Supportees

- `NICK` - Definir le pseudo
- `USER` - Definir le nom d'utilisateur
- `JOIN` - Rejoindre un canal
- `PRIVMSG` - Envoyer des messages prives
- `MODE` - Definir les modes canal/utilisateur
- `TOPIC` - Voir/definir le sujet du canal
- `KICK` - Expulser un utilisateur du canal
- `INVITE` - Inviter un utilisateur dans un canal
- `PING/PONG` - Maintien de connexion

## Compilation

```bash
make
```

## Utilisation

```bash
./ft_irc <port> <mot_de_passe>
```

Exemple:
```bash
./ft_irc 6667 monmotdepasse
```

Connectez-vous ensuite avec un client IRC (irssi, WeeChat, HexChat, etc.).

## Auteurs

- maderuel
- ablancha
- olcoste
