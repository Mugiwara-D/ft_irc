/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:24:28 by ablancha          #+#    #+#             */
/*   Updated: 2024/10/02 15:34:09 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "client.hpp"

class channel

{
private:
	std::string name;
	std::string	_topic;
	std::string	_key;
	std::string	_operator;
	bool		_inviteOnly;
	bool		_lockTopic;
	bool		_locked;
	int			_userLimit;
	
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
	void		setOps( std::string ops );
};

#endif
