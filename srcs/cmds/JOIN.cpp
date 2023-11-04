/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:45:07 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/04 15:23:56 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::joinCommand(int i, t_fd fd, t_params params)
{
	// check if the client is not registered
	if (!clients[i][fd].isRegistered())
	{
		// send irc server error message
		sendData(fd, ERR_NOTREGISTERED(clients[i][fd].getNickname()));
		return ;
	}

	// check if invalid number of params
	if (params.size() > 3)
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), params[0]));
		return ;
	}

	// create a channel object
	Channel channel;

	// channel name
	std::string channelName = params[1];
	// if channel name dose not start with '#', add '#' to the channel name
	if (channelName[0] != '#')
		channelName = "#" + channelName;

	// add the channel name to the channel object
	channel.setName(channelName);
	// add the client to the channel
	channel.addClient(clients[i][fd]);
	// add the first client to the operator list
	channel.addOperator(clients[i][fd]);
	// add channel to the channels vector
	if (nbChannels < MAX_CHANNELS)
	{
		channels[nbChannels].push_back(channel);
		nbChannels++;
	}
	else
	{
		// send irc server error message
		sendData(fd, ERR_TOOMANYCHANNELS(clients[i][fd].getNickname(), channelName));
		return ;
	}
	
	// send irc server message
	
	// send join message to all clients in the channel
	for (size_t j = 0; j < channels[nbChannels - 1].size(); j++)
	{
		for (size_t k = 0; k < channels[nbChannels - 1][j].getClients().size(); k++)
		{
			sendData(channels[nbChannels - 1][j].getClients()[k].getFd(), JOIN(clients[i][fd].getNickname(), clients[i][fd].getUsername(), channelName));
		}
	}
	
	
	// sendData(fd, RPL_TOPIC(clients[i][fd].getNickname(), channelName, channel.getTopic()));
	// sendData(fd, RPL_NAMREPLY(clients[i][fd].getNickname(), channelName, channel.getClients()));
	sendData(fd, RPL_ENDOFNAMES(clients[i][fd].getNickname(), channelName));
}

