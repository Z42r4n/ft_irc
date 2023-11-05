/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:45:07 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/05 17:22:16 by ymoutaou         ###   ########.fr       */
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
	if (params.size() < 2 || params.size() > 3)
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), params[0]));
		return ;
	}

	// check if the name of channe not start with '#'
	if (params[1][0] != '#')
	{
		// send irc server error message
		sendData(fd, ERR_NOSUCHCHANNEL(clients[i][fd].getNickname(), params[1]));
		return ;
	}
	
	// check if the channel exist return -1 if not exist
	int channelIndex = channelExist(params[1]);
	
	// the channel not exist
	if (channelIndex == -1)
	{
		// create new channel
		Channel channel;
		
		// set channel name of cchannel
		channel.setName(params[1]);
		
		// add the name of the channel to the channels vector
		clients[i][fd].addChannel(channelIndex);
		
		// add the client to the channel
		channel.addClient(clients[i][fd]);
		
		// add channel to the channels vector
		if (nbChannels < MAX_CHANNELS)
		{
			channels.push_back(channel);
			nbChannels++;
		}
		else
		{
			// send irc server error message
			sendData(fd, ERR_TOOMANYCHANNELS(clients[i][fd].getNickname(), params[1]));
			return ;
		}
		
		// send irc server message
		sendData(fd, JOIN(clients[i][fd].getNickname(), clients[i][fd].getUsername(), params[1]));
		sendData(fd, RPL_NAMREPLY(clients[i][fd].getNickname(), params[1], channel.listClients()));
		sendData(fd, RPL_ENDOFNAMES(clients[i][fd].getNickname(), params[1]));
		return ;
	}
	
	// here the channel exist

	// check if the client is already joined to the channel
	if (channels[channelIndex].clientExist(clients[i][fd]))
	{
		// this block for later
		return ;
	}
	
	// add the name of the channel to the channels vector
	clients[i][fd].addChannel(channelIndex);
	
	// add the client to the channel
	channels[channelIndex].addClient(clients[i][fd]);
	
	// send irc server message to clients in the channel
	channelBroadcast(i, fd, params, channelIndex, _JOIN);
	
	sendData(fd, RPL_NAMREPLY(clients[i][fd].getNickname(), params[1], channels[channelIndex].listClients()));
	sendData(fd, RPL_ENDOFNAMES(clients[i][fd].getNickname(), params[1]));
}

