/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 09:13:44 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/11 14:24:11 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::userCommand(int i, t_fd fd, t_params params)
{
	// ths user command can be used only after the password command
	if (clients[fd].isGetPassword())
	{
		// check if the client is already registered
		if (clients[fd].getUsername() != "")
		{
			// send irc server error message
			sendData(fd, ERR_ALREADYREGISTERED(clients[fd].getNickname()));
			return ;
		}
		
		// check if nickname is empty
		if (params.size() != 5 && params[4].find(":") == std::string::npos)
		{
			// send irc server error message
			sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
			return ;
		}

		// check if the username is too long
		if (params[1].length() > USERLEN)
		{
			// truncate the username to USERLEN
			params[1] = params[1].substr(0, USERLEN);
		}
		
		// if the username containe @ symbol, we need to get the username before the @ symbol
		if (params[1].find("@") != std::string::npos)
			params[1] = params[1].substr(0, params[1].find("@"));

		// set the username
		clients[fd].setUsername(params[1]);

		// set the realname
		clients[fd].setRealname(params[4]);

		// if the realname contains spaces, we need to get the realname after the colon symbol
		if (params[4].find(":") != std::string::npos)
			clients[fd].setRealname(ft::ft_getStr(params));

		// set the client as registered if the username and nickname are set
		if (clients[fd].getUsername() != "" && clients[fd].getNickname() != "*")
		{
			clients[fd].setIsRegistered(true);

			// send irc server welcome message
			welcomeMessage(fd);
		}
	}
	else
	{
		// send irc server error message bad password
		sendData(fd, ERROR(std::string("Access denied: Bad password?")));

		// clode connection
		closeConnection(i, fd);
	}
}