/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablancha <ablancha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:32:27 by ablancha          #+#    #+#             */
/*   Updated: 2024/08/28 14:59:53 by ablancha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <csignal>
#include "server.hpp"
#include "client.hpp"

bool server_running = true;

bool isnum(char *str)
{
	int i =0;
	while(str[i] != 0)
	{
		if(str[i] < '0' || str[i] > '9')
			return(false);
		i++;
	}
	return(true);
}

int validport(char *port)
{
	if((isnum(port) == false))
		{
			std::cout << "Port shall be positive numeric value  only" << std::endl;
			return(-1);
		}
		int Iport = atoi(port);
		if(Iport > 65535 || Iport < 0)
		{
			std::cout << "Port shall be lower than 65535 and positiv" << std::endl;
			return(-1);
		}
		std::cout << "port :" << Iport << std::endl;
		return(Iport);
}

static void	signalHandler(int sig) {
	std::cout << "\nInterupt signal (" << sig << ") received." << std::endl;
	server_running = false;
}

int	main(int argc, char **argv)
{
	signal(SIGINT, signalHandler);

	if(argc != 3)
	{
		std::cout << "Two arguments required" << std::endl;	
	}
	else
	{
		int port = validport(argv[1]);
		if(port != -1)
		{
			std::cout << "good port" << std::endl;
			Server Server(port, argv[2]);
			Server.start();
		}
	}
		
}
