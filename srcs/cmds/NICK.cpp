/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 14:15:54 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/11 14:41:42 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::nickCommand(int i, t_fd fd, t_params params)
{
	// ths nick command can be used only after the password command
	if (clients[fd].isGetPassword())
	{
		// check if nickname is empty
		if (params.size() != 2)
		{
			// send irc server error message
			sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
			return ;
		}
		
		// check if the nickname is too long
		if (params[1].length() > 9)
		{
			// send irc server error message
			sendData(fd, ERR_NICKNAMETOOLONG(clients[fd].getNickname(), params[1]));
			return ;
		}
		
		//check if the nick is valid
		if (!ft::isNicknameValid(params[1]))
		{
			// send irc server error message
			sendData(fd, ERR_ERRONEUSNICKNAME(clients[fd].getNickname(), params[1]));
			return ;
		}	

		// check if the nickname is already taken from another client
		for (t_fd k = 0; k < nfds; k++)
		{
			for (t_fd j = 0; j < (t_fd)clients.size(); j++)
			{
				if (k != j && clients[j].getNickname() == params[1])
				{
					// send irc server error message
					sendData(fd, ERR_NICKNAMEINUSE(clients[fd].getNickname(), params[1]));
					return ;
				}
			}
		}
		
		// if the client is already registered
		if (clients[fd].isRegistered())
		{
			if (clients[fd].getNickname() != params[1])
			{
				// check if channel has clients
				if (clients[fd].getChannelsSize() > 0)
				{
					// send broadcast message to all clients in channels that the client joined
					for (size_t j = 0; j < clients[fd].getChannelsSize(); j++)
					{
						channelBroadcast(fd, params[1], clients[fd].getChannel(j), _NICK);
					}
					for (size_t j = 0; j < clients[fd].getChannelsSize(); j++)
					{
						for (size_t k = 0; k < channels[clients[fd].getChannel(j)].getClientsSize(); k++)
						{
							channels[clients[fd].getChannel(j)].getClient(k)->setIsReceivedMsg(false);
						}
					}
				}
				else
				{
					sendData(fd, NICK(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp() ,params[1]));
				}
			}
			
			// set the new nickname
			clients[fd].setNickname(params[1]);
			
			return ;
		}
		
		// set the nickname
		clients[fd].setNickname(params[1]);

		// set the client as registered if the username and nickname are set
		if (clients[fd].getUsername() != "" && clients[fd].getNickname() != "*")
		{
			clients[fd].setIsRegistered(true);
			
			// send welcome message
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
	