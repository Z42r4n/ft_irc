/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 14:11:47 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/14 13:34:06 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ircserv.hpp>

void Server::quitCommand(int i, t_fd fd, t_params params)
{
	// check if invalid number of params
	if (params.size() < 1)
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
		return ;
	}

	// check if th params > 2, need the second param to start with ':'
	if (params.size() > 2 && params[1][0] != ':')
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
		return ;
	}

	// stor the reason
	std::string reason = "Closing connection";
	if (params.size() > 2 && params[1][0] == ':')
	{
		reason = ft::ft_getStr(params);
		reason = '"' + reason + '"';
	}
	else if (params.size() == 2)
	{
		reason = params[1];
		if (reason[0] == ':')
			reason.erase(0, 1);
		reason = '"' + reason + '"';
	}

	// check if the client is not registered
	if (!clients[fd].isRegistered())
	{
		// send irc server message
		sendData(fd, ERROR(std::string(reason )));
		
		// close the connection
		closeConnection(i, fd);
	}
	else
	{
		// send notice message to client
		sendData(fd, NOTICE(clients[fd].getNickname()));

		// send irc server message
		sendData(fd, ERROR(std::string(reason)));

		// update the reason
		if (reason == "Closing connection")
			reason = clients[fd].getNickname();

		// sond quit messaget to all channels
		for (size_t j = 0; j < clients[fd].getChannelsSize(); j++)
		{
			// broadcast message to all clients in channel
			channelBroadcast(fd, reason, clients[fd].getChannel(j), _QUIT);
			// remove the client from the channel
			
			std::cout << "channel index: " << clients[fd].getChannel(j) << std::endl;
			channels[j].removeClient(&clients[fd]);
			// remove the client from operators, if the client is the operator of the channel
			if (channels[clients[fd].getChannel(j)].isOperator(clients[fd]))
				channels[clients[fd].getChannel(j)].removeOperator(&clients[fd]);
			// remove the index of the channel from the client
			clients[fd].removeChannel(clients[fd].getChannel(j));
		}
		// print the names of channels and clients
		for (size_t j = 0; j < channels.size(); j++)
		{
			std::cout << "channel name: " << channels[j].getName() << std::endl;
			std::cout << "channel clients: " << std::endl;
			for (size_t k = 0; k < channels[j].getClientsSize(); k++)
			{
				std::cout << channels[j].getClient(k)->getNickname() << std::endl;
			}
		}
		// close the connection
		closeConnection(i, fd);
	}
}