/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:45:07 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/15 15:16:05 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::joinCommand(t_fd fd, t_params params)
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
	
	// split the params[1] by ',' if the client want to join multiple channels
	t_params chans;
	
	if (params[1].find(',') != std::string::npos)
	{
		chans = ft::ft_split(params[1], ",");
		
		// check if the name of the channel is duplicated
		for (size_t j = 0; j < chans.size(); j++)
		{
			for (size_t k = 0; k < chans.size(); k++)
			{
				if (j != k && chans[j] == chans[k])
				{
					// send irc server error message
					chans.erase(chans.begin() + k);
					k = 0;
				}
			}
		}
	}
	else
	{
		chans.push_back(params[1]);
	}

	// split the params[2] by ',' if the client want to join multiple channels
	t_params keys;
	
	if (params.size() == 3)
	{
		if (params[2].find(',') != std::string::npos)
		{
			keys = ft::ft_split(params[2], ",");
		}
		else
		{
			keys.push_back(params[2]);
		}
	}

	// check if the channels exist return -1 if not exist
	std::vector<int> chansIndex;
	
	if (chans.size() > 1)
	{
		for (size_t j = 0; j < chans.size(); j++)
		{
			
			chansIndex.push_back(channelExist(chans[j]));
		}
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
			sendData(fd, ERR_NOSUCHCHANNEL(clients[fd].getNickname(), chans[j]));
			continue ;
		}

		// check if the channel not exist
		if (chansIndex[j] == -1)
		{
			// create new channel with the name of the channel
			Channel channel(chans[j]);
			
			// add the index of the channel to the channels vector
			clients[fd].addChannel(nbChannels);
			
			// add the client to the channel
			channel.addClient(&(clients[fd]));

			// set the fist client as operator
			channel.addOperator(&(clients[fd]));
			
			// add channel to the channels vector
			if (nbChannels < MAX_CHANNELS)
			{
				channels.push_back(channel);
				nbChannels++;
			}
			else
			{
				// send irc server error message
				sendData(fd, ERR_TOOMANYCHANNELS(clients[fd].getNickname(), chans[j]));
				return ;
			}
			
			// send irc server message
			sendData(fd, JOIN(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), chans[j]));
			sendData(fd, RPL_NAMREPLY(clients[fd].getNickname(), chans[j], channel.listClients()));
			sendData(fd, RPL_ENDOFNAMES(clients[fd].getNickname(), chans[j]));
			continue ;
		}
		// here the channel or channels exist

		// check if the client is already joined to the channel
		if (channels[chansIndex[j]].clientExist(clients[fd]))
		{
			// this block for later
			return ;
		}

		// check if the channel is invite only
		if (channels[chansIndex[j]].getModes().find('i') != std::string::npos)
		{
			// check if the client is invited
			if (!channels[chansIndex[j]].isInvited(clients[fd]))
			{
				// send irc server error message
				sendData(fd, ERR_INVITEONLYCHAN(clients[fd].getNickname(), chans[j]));
				continue ;
			}
		}

		// check if the channel has limit
		if (channels[chansIndex[j]].getModes().find('l') != std::string::npos)
		{
				// check if the channel is full
			if (channels[chansIndex[j]].getClientsSize() + 1 > channels[chansIndex[j]].getLimit())
			{
				// send irc server error message
				sendData(fd, ERR_CHANNELISFULL(clients[fd].getNickname(), chans[j]));
				continue ;
			}
		}

		// check if the channel has password
		if (channels[chansIndex[j]].getModes().find('k') != std::string::npos)
		{
			// check if the client send the password
			if (params.size() == 3)
			{
				// check if the password is correct
				if (keys[j] != channels[chansIndex[j]].getKey())
				{
					// send irc server error message
					sendData(fd, ERR_BADCHANNELKEY(clients[fd].getNickname(), chans[j]));
					continue ;
				}
			}
			else
			{
				// send irc server error message
				sendData(fd, ERR_BADCHANNELKEY(clients[fd].getNickname(), chans[j]));
				continue ;
			}
		}
		
		// add the name of the channel to the channels vector
		clients[fd].addChannel(chansIndex[j]);
		
		// add the client to the channel
		channels[chansIndex[j]].addClient(&(clients[fd]));
		
		// send irc server message to clients in the channel
		channelBroadcast(fd, chans[j], chansIndex[j], _JOIN);
		
		sendData(fd, RPL_NAMREPLY(clients[fd].getNickname(), chans[j], channels[chansIndex[j]].listClients()));
		sendData(fd, RPL_ENDOFNAMES(clients[fd].getNickname(), chans[j]));
	}
}
	

