/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 10:42:22 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/13 13:11:42 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::inviteCommand(t_fd fd, t_params params)
{
	// check if the client is not registered
	if (!clients[fd].isRegistered())
	{
		// send irc server error message
		sendData(fd, ERR_NOTREGISTERED(clients[fd].getNickname()));
		return ;
	}

	// check if invalid number of params
	if (params.size() != 3)
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
		return ;
	}

	// check if the invited user exist in the server
	if (getClientFd(params[1]) == -1)
	{
		// send irc server error message
		sendData(fd, ERR_NOSUCHNICK(clients[fd].getNickname(), params[1]));
		return ;
	}

	// check if the channel exist
	if (channelExist(params[2]) == -1)
	{
		// send irc server error message
		sendData(fd, ERR_NOSUCHCHANNEL(clients[fd].getNickname(), params[2]));
		return ;
	}

	// check if the user already in the channel
	if (channels[channelExist(params[2])].clientExist(clients[getClientFd(params[1])]))
	{
		// send irc server error message
		sendData(fd, ERR_USERONCHANNEL(clients[fd].getNickname(), params[1], params[2]));
		return ;
	}

	printf("before check if the channel is invite only\n");

	// check if the channel is invite only
	if (channels[channelExist(params[2])].getModes().find('i') != std::string::npos)
	{
		// check if the client is operator
		if (!channels[channelExist(params[2])].isOperator(clients[fd]))
		{
			// send irc server error message
			sendData(fd, ERR_CHANOPRIVSNEEDED(clients[fd].getNickname(), params[2]));
			return ;
		}
		// add the user to invited list
		channels[channelExist(params[2])].addInvited(&(clients[getClientFd(params[1])]));
		
		// send irc invite message to the one who invited
		sendData(fd, RPL_INVITING(params[1], clients[getClientFd(params[1])].getUsername(), clients[getClientFd(params[1])].getIp(), clients[fd].getNickname(), params[1], params[2]));

		// send irc invite message to the invited user
		sendData(getClientFd(params[1]), INVITE(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), params[1], params[2]));
	}
	else
	{
		printf("before add invited\n");
		// add the user to invited list
		channels[channelExist(params[2])].addInvited(&(clients[getClientFd(params[1])]));
		// printf for debug
		printf("after add invited\n");
		
		// send irc invite message to the one who invited
		sendData(fd, RPL_INVITING(params[1], clients[getClientFd(params[1])].getUsername(), clients[getClientFd(params[1])].getIp(), clients[fd].getNickname(), params[1], params[2]));

		// send irc invite message to the invited user
		sendData(getClientFd(params[1]), INVITE(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), params[1], params[2]));
	}
}