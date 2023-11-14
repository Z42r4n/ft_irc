/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:36:23 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/14 13:05:24 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::partCommand(t_fd fd, t_params params)
{
	// check if the client is not registered
	if (!clients[fd].isRegistered())
	{
		// send irc server error message
		sendData(fd, ERR_NOTREGISTERED(clients[fd].getNickname()));
		return ;
	}

	// check if invalid number of params
	if (params.size() < 2)
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

	// store the reason
	std::string reason = "";

	if (params.size() > 3 && params[2][0] == ':')
	{
		reason = ft::ft_getStr(params);
	}
	else if (params.size() == 3)
	{
		// skip the first : in the reason
		reason = params[2];
		if (params.size() == 3 && params[2][0] == ':')
			reason = params[2].substr(1);
	}

	// store the channels
	t_params chans;
	if (params[1].find(',') != std::string::npos)
	{
		chans = ft::ft_split(params[1], ",");
	}
	else
	{
		chans.push_back(params[1]);
	}

	// exit the channel and send irc server message
	for (size_t i = 0; i < chans.size(); i++)
	{
		// check if the channel exist
		if (channelExist(chans[i]) != -1)
		{
			// check if the client is in the channel
			if (clients[fd].isInChannel(channelExist(chans[i])))
			{
				// broadcast message to all clients in channel
				channelBroadcast(fd, chans[i], channelExist(chans[i]), _PART, reason);
				
				// remove the client from the channel
				channels[channelExist(chans[i])].removeClient(&clients[fd]);

				// remove client from operator list if is operator
				if (channels[channelExist(chans[i])].isOperator(clients[fd]))
					channels[channelExist(chans[i])].removeOperator(&clients[fd]);

				// remove the index of the channel from the client
				clients[fd].removeChannel(channelExist(chans[i]));

				// remove the client from the invited list if is invited
				if (channels[channelExist(chans[i])].isInvited(clients[fd]))
				{
					channels[channelExist(chans[i])].removeInvited(&clients[fd]);
				}
			}
			else
			{
				// send irc server error message
				sendData(fd, ERR_NOTONCHANNEL(clients[fd].getNickname(), chans[i]));
			}
		}
		else
		{
			// send irc server error message
			sendData(fd, ERR_NOSUCHCHANNEL(clients[fd].getNickname(), chans[i]));
		}
	}
}