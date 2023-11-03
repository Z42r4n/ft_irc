/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 14:15:54 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/03 12:13:32 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

namespace ft
{
	bool isNicknameValid(std::string nickname)
	{
		std::string chars = "[]\\`_^{|}";
		
		// check if nickname is valid
		for (size_t i = 0; i < nickname.length(); i++)
		{
			//check if the nickname not start with acceptable chars
			if (i == 0 && nickname[i] != '_' && !std::isalpha(nickname[i]))
			{
				return false;
			}
			
			// check if the nickname contains only acceptable chars
			if (!std::isalnum(nickname[i]) && chars.find(nickname[i]) == std::string::npos)
			{
				return false;
			}
		}
		
		return true;
	}
}

void Server::nickCommand(int i, t_fd fd, t_params params)
{
	// ths nick command can be used only after the password command
	if (clients[i][fd].isGetPassword())
	{
		// check if nickname is empty
		if (params.size() != 2)
		{
			// send irc server error message
			sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), params[0]));
			return ;
		}
		
		// check if the nickname is too long
		if (params[1].length() > 9)
		{
			// send irc server error message
			sendData(fd, ERR_NICKNAMETOOLONG(clients[i][fd].getNickname(), params[1]));
			return ;
		}
		
		//check if the nick is valid
		if (!ft::isNicknameValid(params[1]))
		{
			// send irc server error message
			sendData(fd, ERR_ERRONEUSNICKNAME(clients[i][fd].getNickname(), params[1]));
			return ;
		}	

		// check if the nickname is already taken from another client
		for (t_fd k = 0; k < nfds; k++)
		{
			for (size_t j = 0; j < clients[k].size(); j++)
			{
				if (k != i && clients[k][j].getNickname() == params[1])
				{
					// send irc server error message
					sendData(fd, ERR_NICKNAMEINUSE(clients[k][fd].getNickname(), params[1]));
					return ;
				}
			}
		}
		
		// if the client is already registered
		if (clients[i][fd].isRegistered())
		{
			if (clients[i][fd].getNickname() != params[1])
			{
				// respect this format "":nickName!~userName@localhost NICK :newNickname\r\n"
				sendData(fd, ":" + clients[i][fd].getNickname() + "!~" + clients[i][fd].getUsername() + "@localhost NICK :" + params[1] + "\r\n");
			}
			
			// set the new nickname
			clients[i][fd].setNickname(params[1]);
			return ;
		}
		
		// set the nickname
		clients[i][fd].setNickname(params[1]);

		// set the client as registered if the username and nickname are set
		if (clients[i][fd].getUsername() != "" && clients[i][fd].getNickname() != "*")
		{
			clients[i][fd].setIsRegistered(true);
			
			// send welcome message
			welcomeMessage(i, fd);
		}
	}
}
	