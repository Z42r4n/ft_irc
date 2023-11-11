/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PMSG.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 18:16:11 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/11 13:08:15 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::privmsgCommand(t_fd fd, t_params params)
{
	// check if the client is not registered
	if (!clients[fd].isRegistered())
	{
		// send irc server error message
		sendData(fd, ERR_NOTREGISTERED(clients[fd].getNickname()));
		return ;
	}

	// check if invalid number of params
	if (params.size() < 3)
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
		return ;
	}

	// if the number of params more than 3, the third param shold start with ':'
	if (params.size() > 3 && params[2][0] != ':')
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
		return ;
	}

	// store the message
	std::string message = params[2];
	if (params[2][0] == ':')
	{
		// get the message
		message = ft::ft_getStr(params);
	}

	// store the target
	t_params targets;
	if (params[1].find(',') != std::string::npos)
	{
		targets = ft::ft_split(params[1], ",");
	}
	else
	{
		targets.push_back(params[1]);
	}
	
	// loop through the targets and send the message if the target is found
	for (size_t i = 0; i < targets.size(); i++)
	{
		// check if the target is a channel
		if (targets[i][0] == '#' && channelExist(targets[i]) != -1)
		{
			// check if the client is in the channel
			if (clients[fd].isInChannel(channelExist(targets[i])))
			{
				// send the message to all clients in the channel
				channelBroadcast(fd, targets[i], channelExist(targets[i]), _MSG, message);
			}
			else
			{
				// send irc server error message
				sendData(fd, ERR_CANNOTSENDTOCHAN(clients[fd].getNickname(), targets[i]));
			}
		}
		else
		{
			// check if the target is a client
			t_fd targetFd = getClientFd(targets[i]);
			if (targetFd != -1)
			{
				// send the message to the target
				if (!clients[fd].isReceivedMsg())
				{
					sendData(targetFd, PRIVMSG(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), targets[i], message));
					clients[fd].setIsReceivedMsg(true);
				}
			}
			else
			{
				// send irc server error message
				sendData(fd, ERR_NOSUCHNICK(clients[fd].getNickname(), targets[i]));
			}
		}
	}
	// reset the isReceivedMsg of the client to false
	for (size_t j = 0; j < clients.size(); j++)
	{
		clients[j].setIsReceivedMsg(false);
	}
}