/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:20:58 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/09 12:55:47 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::modeCommand(t_fd fd, t_params params)
{
	// checj if client is registered
	if (!clients[fd].isRegistered())
	{
		// send irc server error message
		sendData(fd, ERR_NOTREGISTERED(clients[fd].getNickname()));
		return ;
	}

	// check if invalid number of params
	if (params.size() < 2 || params.size() > 4)
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

	// check if the mode not set
	if (params.size() == 2)
	{
		// send irc server error message
		sendData(fd, RPL_CHANNELMODEIS(clients[fd].getNickname(), params[1]));
		sendData(fd, RPL_CREATIONTIME(clients[fd].getNickname(), params[1], std::to_string(channels[channelExist(params[1])].getCreationTime())));
	}
	
	// set the default mode
	if (!channels[channelExist(params[1])].modeIsSet('t'))
	{
		std::cout << "set mode t" << std::endl;
		channels[channelExist(params[1])].addMode('t');
		sendData(fd, MODE(clients[fd].getNickname(), clients[fd].getUsername(), params[1], "+t"));
	}

	// this block of code in case of the mode is setted
	if (params.size() > 2)
	{
		std::string modeWasSet = "+";
		// check if the mode is valid
		for (size_t j = 0; j < params[2].length(); j++)
		{
			//check if th mode string start with '+' or '-'
			// if (j == 0 && params[2][j] != '+' && params[2][j] != '-')
			// {
			// 	// send irc server error message
			// 	sendData(fd, ERR_UNKNOWNMODE(clients[fd].getNickname(), params[2]));
			// 	return ;
			// }
			
			// extract the mode if +++++il-------k well be "+i", "+l", "-k"
			// if (params[2][j] == '+' || params[2][j] == '-')
			// {
			// 	modeWasSet += params[2][j];
			// 	continue ;
			// }
			
			
			
			if (!ft::validModes(params[2][j]))
			{
				// send irc server error message
				sendData(fd, ERR_UNKNOWNMODE(clients[fd].getNickname(), params[1]));

			}
			// check if the mode already setted
			else if (channels[channelExist(params[1])].modeIsSet(params[2][j]))
			{
				// skip the mode that already setted
				continue ;
			}
			else
			{
				// setting the mode
				modeWasSet += params[2][j];
				channels[channelExist(params[1])].addMode(params[2][j]);
			}
		}
		if (modeWasSet.length() != 1)
			sendData(fd, MODE(clients[fd].getNickname(), clients[fd].getUsername(), params[1], modeWasSet));
	}
}