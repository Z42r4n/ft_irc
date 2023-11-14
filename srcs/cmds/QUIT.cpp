/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 14:11:47 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/14 10:21:53 by ymoutaou         ###   ########.fr       */
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
			channels[clients[fd].getChannel(j)].removeClient(&clients[fd]);
			// remove the client from operators, if the client is the operator of the channel
			if (channels[clients[fd].getChannel(j)].isOperator(clients[fd]))
				channels[clients[fd].getChannel(j)].removeOperator(&clients[fd]);
			// remove the index of the channel from the client
			clients[fd].removeChannel(clients[fd].getChannel(j));
		}
		// close the connection
		closeConnection(i, fd);
	}






	
	// std::string quit_message = "Closing connection";
	
	// // check if invalid number of params
	// if (params.size() > 2 && params[1].find(":") == std::string::npos)
	// {
	// 	// send irc server error message
	// 	sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
	// 	return ;
	// }
	
	// // get the message after the colon symbol
	// if (params[1].find(":") != std::string::npos)
	// {
	// 	// print params[1] length
	// 	if (params[1].length() > 1)
	// 		quit_message = ft::ft_getStr(params);
	// }
	// else if (params.size() == 2)
	// {
	// 	// get the message
	// 	quit_message = params[1];
	// }

	// // if client is registered send notice message
	// if (clients[fd].isRegistered())
	// {
	// 	// send notice message to client
	// 	sendData(fd, NOTICE(clients[fd].getNickname()));
		
	// 	// send quit message to all clients in the same channel
	// 	if (clients[fd].getChannelsSize() > 0)
	// 	{	
	// 		for (size_t j = 0; j < clients[fd].getChannelsSize(); j++)
	// 		{
	// 			// get the index of the channel
	// 			size_t channelIndex = clients[fd].getChannel(j);
				
	// 			// broadcast message to all clients in channel
	// 			channelBroadcast(fd, "Test", channelIndex, _QUIT);
	// 		}
			
	// 		// remove the client from all channels
	// 		// for (size_t j = 0; j < clients[fd].getChannelsSize(); j++)
	// 		// {
	// 		// 	// get the index of the channel
	// 		// 	size_t channelIndex = clients[fd].getChannel(j);
				
	// 		// 	// remove the client from the channel
	// 		// 	channels[channelIndex].removeClient(clients[fd]);
	// 		// }
	// 	}
	// }
	
	// // send irc server error message
	// sendData(fd, ERROR(std::string(quit_message)));
	
	// // close the connection
	// closeConnection(i, fd);
}