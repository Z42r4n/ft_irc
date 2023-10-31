/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:33:35 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/10/31 14:19:16 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::passCommand(int i, t_fd fd, std::string command, std::string param)
{
	std::string pass = "";
	
	// check if the client is already registered
	if (clients[i][fd].isRegistered())
	{
		// send irc server error message
		sendData(fd, ERR_ALREADYREGISTERED(clients[i][fd].getNickname()));
		return ;
	}
	
	// check empty password
	if (param.empty())
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), command));
		return ;
	}
	
	// find the tow dots position
	if (param.find(":") != std::string::npos)
	{
		// get the password
		pass = param.substr(param.find(":") + 1);
	}
	else if ((param.find(" ") != std::string::npos))
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), command));
		return;
	}
	
	if (this->password == pass)
	{
		// send irc server error message
		clients[i][fd].setIsGetPassword(true);
	}
	else
	{
		// send irc server error message
		this->sendData(fd, ERR_PASSWDMISMATCH);
	}
}