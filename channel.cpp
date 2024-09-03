/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:07:08 by ablancha          #+#    #+#             */
/*   Updated: 2024/08/27 14:07:10 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

// Constructor
channel::channel(std::string name) : name(name)
{
    std::cout << "Channel \"" << name << "\" created." << std::endl;
}

// Destructor
channel::~channel()
{
    std::cout << "Channel \"" << name << "\" destroyed." << std::endl;
}