/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:45:07 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/07 11:12:07 by ymoutaou         ###   ########.fr       */
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
	
	// split the params[1] by ',' if the client want to join multiple channels
	t_params chans;
	
	if (params[1].find(',') != std::string::npos)
	{
		chans = ft::ft_split(params[1], ",");
		
		// check if channels is duplicated
		for (size_t j = 0; j < chans.size(); j++)
		{
			for (size_t k = j + 1; k < chans.size(); k++)
			{
				if (chans[j] == chans[k])
				{
					// put in chans only one time
					chans.erase(chans.begin() + k);
				}
			}
		}

	}
	else
	{
		chans.push_back(params[1]);
	}

	// check if the channels exist return -1 if not exist
	std::vector<int> chansIndex;
	
	if (chans.size() > 1)
	{
		for (size_t j = 0; j < chans.size(); j++)
			chansIndex.push_back(channelExist(chans[j]));
	}
	else
	{
		chansIndex.push_back(channelExist(chans[0]));
	}
	
	for (size_t j = 0; j < chansIndex.size(); j++)
	{
		// check if the channel start with '#'
		if (chans[j][0] != '#')
		{
			// send irc server error message
			sendData(fd, ERR_NOSUCHCHANNEL(clients[i][fd].getNickname(), chans[j]));
			continue ;
		}

		// check if the channel not exist
		if (chansIndex[j] == -1)
		{
			// create new channel with the name of the channel
			Channel channel(chans[j]);
			
			// add the index of the channel to the channels vector
			clients[i][fd].addChannel(nbChannels);
			
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
				sendData(fd, ERR_TOOMANYCHANNELS(clients[i][fd].getNickname(), chans[j]));
				return ;
			}
			
			// send irc server message
			sendData(fd, JOIN(clients[i][fd].getNickname(), clients[i][fd].getUsername(), chans[j]));
			sendData(fd, RPL_NAMREPLY(clients[i][fd].getNickname(), chans[j], channel.listClients()));
			sendData(fd, RPL_ENDOFNAMES(clients[i][fd].getNickname(), chans[i]));
			continue ;
		}
		// here the channel or channels exist

		// check if the client is already joined to the channel
		if (channels[chansIndex[j]].clientExist(clients[i][fd]))
		{
			// this block for later
			return ;
		}
		
		// add the name of the channel to the channels vector
		clients[i][fd].addChannel(chansIndex[j]);
		
		// add the client to the channel
		channels[chansIndex[j]].addClient(clients[i][fd]);
		
		// send irc server message to clients in the channel
		channelBroadcast(i, fd, chans[j], chansIndex[j], _JOIN);
		
		sendData(fd, RPL_NAMREPLY(clients[i][fd].getNickname(), chans[j], channels[chansIndex[j]].listClients()));
		sendData(fd, RPL_ENDOFNAMES(clients[i][fd].getNickname(), chans[j]));
	}
}
	

