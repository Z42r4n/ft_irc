/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 10:10:49 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/12 14:03:59 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ircserv.hpp>

void Server::topicCommand(t_fd fd, t_params params)
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

	// params > 3
	if (params.size() > 3 && params[2][0] != ':')
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
		return ;
	}

	// check if channel exist
	if (channelExist(params[1]) == -1)
	{
		// send irc server error message
		sendData(fd, ERR_NOSUCHCHANNEL(clients[fd].getNickname(), params[1]));
		return ;
	}

	// check if the client is not in the channel
	if (!channels[channelExist(params[1])].clientExist(clients[fd]))
	{
		// send irc server error message
		sendData(fd, ERR_NOTONCHANNEL(clients[fd].getNickname(), params[1]));
		return ;
	}

	// return the topic if params.size() == 2
	if (params.size() == 2)
	{
		// print the topic for debug
		sendData(fd, RPL_TOPIC(clients[fd].getNickname(), params[1], channels[channelExist(params[1])].getTopic()));
		if (channels[channelExist(params[1])].getTopic() != "No topic is set")
			sendData(fd, RPL_TOPICWHOTIME(clients[fd].getNickname(), params[1], clients[fd].getNickname(), std::to_string(channels[channelExist(params[1])].getTopicTime())));
		return ;
	}

	std::string topic = params[2];
	if (params.size() > 2 && params[2][0] == ':')
	{
		topic = ft::ft_getStr(params);
	}

	// check if t mode is set
	if (channels[channelExist(params[1])].modeIsSet('t'))
	{
		// check if the client is not operator
		if (!channels[channelExist(params[1])].isOperator(clients[fd]))
		{
			// send irc server error message
			sendData(fd, ERR_CHANOPRIVSNEEDED(clients[fd].getNickname(), params[1]));
		}
		else
		{
			// set the topic if not empty
			if (channels[channelExist(params[1])].getTopic() != topic)
			{
					// set the topic
				channels[channelExist(params[1])].setTopic(topic);
				// set the topic time
				channels[channelExist(params[1])].setTopicTime(time(NULL));
				// send irc server error message to all clients in the channel
				channelBroadcast(fd, params[1], channelExist(params[1]), _TOPIC, topic);
			}
		}
	}
	else
	{
		if (channels[channelExist(params[1])].getTopic() != topic)
		{
			// set the topic
			channels[channelExist(params[1])].setTopic(topic);
			// set the topic time
			channels[channelExist(params[1])].setTopicTime(time(NULL));
			// send irc server error message to all clients in the channel
			channelBroadcast(fd, params[1], channelExist(params[1]), _TOPIC, topic);
		}
	}
}