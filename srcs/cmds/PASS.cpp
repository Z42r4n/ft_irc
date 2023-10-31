/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:33:35 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/10/31 19:00:53 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::passCommand(int i, t_fd fd, t_params params)
{
	std::string pass = params[1];
	
	// check if the client is already registered
	if (clients[i][fd].isRegistered())
	{
		// send irc server error message
		sendData(fd, ERR_ALREADYREGISTERED(clients[i][fd].getNickname()));
		return ;
	}
	
	// check if invalid number of params
	if (params.size() != 2 && params[1].find(":") == std::string::npos)
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), params[0]));
		return ;
	}

	// get the pass after the colon symbol
	if (params[1].find(":") != std::string::npos)
		pass = ft::ft_getStr(params);
	
	// check if the password is correct
	if (this->password == pass)
	{
		// set the isGetPassword to true
		clients[i][fd].setIsGetPassword(true);
	}
	else
	{
		// send irc server error message
		this->sendData(fd, ERR_PASSWDMISMATCH(clients[i][fd].getNickname()));
	}
}