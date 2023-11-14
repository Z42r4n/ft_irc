/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 08:17:35 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/14 10:25:16 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::kickCommand(t_fd fd, t_params params)
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
	if (params.size() > 3 && params[3][0] != ':')
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
		return ;
	}

	// store the comment
	std::string comment = clients[fd].getNickname();
	if (params.size() == 4)
	{
		comment = params[3];
		if (params.size() == 4 && params[3][0] == ':')
			comment = params[3].substr(1);
	}
	else if (params.size() > 4 && params[3][0] == ':')
	{
		comment = ft::ft_getStr(params);
	}
	
	// store the users
	t_params users;
	if (params[2].find(',') != std::string::npos)
	{
		users = ft::ft_split(params[2], ",");
	}
	else
	{
		users.push_back(params[2]);
	}

	// check if the channel exist
	if (channelExist(params[1]) == -1)
	{
		// send irc server error message
		sendData(fd, ERR_NOSUCHCHANNEL(clients[fd].getNickname(), params[1]));
		return ;
	}

	// loop over the users and kick them if they are in the channel and you are an operator
	for (size_t i = 0; i < users.size(); i++)
	{
		// check if the user exist
		if (getClientFd(users[i]) == -1)
		{
			// send irc server error message
			sendData(fd, ERR_NOSUCHNICK(clients[fd].getNickname(), users[i]));
			continue ;
		}

		// check if the user is in the channel
		if (!channels[channelExist(params[1])].clientExist(clients[getClientFd(users[i])]))
		{
			// send irc server error message
			sendData(fd, ERR_USERNOTINCHANNEL(clients[fd].getNickname(), users[i], params[1]));
			continue ;
		}

		// check if the user is an operator
		if (!channels[channelExist(params[1])].isOperator(clients[fd]))
		{
			// send irc server error message
			sendData(fd, ERR_CHANOPRIVSNEEDED2(clients[fd].getNickname(), params[1]));
			continue ;
		}

		// kick the user
		
		// broadcast message to all clients in channel
		channelBroadcast(fd, params[1], channelExist(params[1]), _KICK, users[i], comment);
		
		// remove the client from the channel
		channels[channelExist(params[1])].removeClient(&(clients[getClientFd(users[i])]));
		
		// remove client from operator list
		if (channels[channelExist(params[1])].isOperator(clients[getClientFd(users[i])]))
		{
			channels[channelExist(params[1])].removeOperator(&(clients[getClientFd(users[i])]));
			// remove the channel if the channel is empty
			// if (channels[channelExist(params[1])].getClientsSize() == 0)
			// {
			// 	channels.erase(channels.begin() + channelExist(params[1]));
			// 	nbChannels--;
			// }
		}

		// remove the channel from the client
		clients[getClientFd(users[i])].removeChannel(channelExist(params[1]));

		// remove from invited list if client is invited
		if (channels[channelExist(params[1])].isInvited(clients[getClientFd(users[i])]))
		{
			channels[channelExist(params[1])].removeInvited(&(clients[getClientFd(users[i])]));
		}
	}
}