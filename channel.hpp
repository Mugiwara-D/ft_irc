/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:24:28 by ablancha          #+#    #+#             */
/*   Updated: 2024/08/27 14:07:23 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>

class channel

{
private:
	std::string name;
	
public:
	channel(std::string name);
	~channel();
};

