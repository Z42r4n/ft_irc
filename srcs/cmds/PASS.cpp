/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:33:35 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/10/30 18:18:51 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::passCommand(int i, t_fd fd, std::string param)
{
	(void)i;
    // print param size
	std::cout << "PASS param size: " << param.size() << std::endl;
	// print param
	std::cout << "PASS param: " << param << std::endl;
	// handle ERR_NEEDMOREPARAMS (461)
	if (param.empty())
	{
		// send irc server error message
		sendData(fd, ":localhost 461 * PASS :Syntax error\n");
		return ;
	}
	
	if (password == param)
	{
		// clients[i][fd].setIsRegistered(true);
		std::cout << "client registered" << std::endl;
	}
	else
	{
		this->sendData(fd, "ERROR :Invalid password\n");
	}
}