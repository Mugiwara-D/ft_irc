/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:24:28 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/16 15:54:28 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "client.hpp"

class Client;
class channel

{
private:
	std::string name;
	std::string	_topic;
	std::string	_key;
	bool		_inviteOnly;
	bool		_lockTopic;
	bool		_locked;
	int			_userLimit;
	std::vector<Client*> clientList;
	std::vector<Client*> operators;
	
public:
	channel( std::string name, bool inviteOnly, bool lockTopic);
	~channel();

	std::string	getname();
	bool		getLockTopic();
	bool		getInviteOnly();

	void		setLockTopic( bool val );
	void		setInviteOnly( bool val );
	void		setKey( std::string key );
	void		setLocked( bool val );
	void		setUserLim( int val );
	void addClient(Client& client);
    void removeClient(Client& client);
    std::vector<Client*> getClientList(); 
    std::vector<Client*> getClientList() const; 
	std::string getname() const;
	void addOps(Client& client);
    void removeOps(Client& client);
	 bool isOperator(const Client& client) const;
	
};

#endif
