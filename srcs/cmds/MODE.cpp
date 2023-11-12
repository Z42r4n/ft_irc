/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:20:58 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/12 09:25:28 by ymoutaou         ###   ########.fr       */
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
		sendData(fd, RPL_CHANNELMODEIS(clients[fd].getNickname(), params[1], "+"));
		sendData(fd, RPL_CREATIONTIME(clients[fd].getNickname(), params[1], std::to_string(channels[channelExist(params[1])].getCreationTime())));
	}

	// set the default mode
	if (!channels[channelExist(params[1])].modeIsSet('t'))
	{
		channels[channelExist(params[1])].addMode('t');
		sendData(fd, MODE(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), params[1], "+t"));
	}

	// this block of code in case of the mode is passed with the command
	if (params.size() > 2)
	{

		std::string mode = params[2];
		
		// handle the mode string
		for (size_t j = 0; j < params[2].length(); j++)
		{
			if (params[2][j] == '+' || params[2][j] == '-')
			{
				mode = ft::ft_replace(mode, "++", "+");
				mode = ft::ft_replace(mode, "--", "-");
				mode = ft::ft_replace(mode, "+-", "-");
				mode = ft::ft_replace(mode, "-+", "+");
				continue;
			}
			
			// remove the + or - from the end of the mode
			size_t end = mode.find_last_not_of("+-");
    		if (end != std::string::npos)
        		mode = mode.substr(0, end + 1);
		}

		// print the mode value

		std::cout << "mode: " << mode << std::endl;

		// set the mode
		for (size_t i = 0; i < mode.length(); i++) // first one is +
		{
			
			if (!ft::validModes(mode[i]))
			{
				// send irc server error message
				sendData(fd, ERR_UNKNOWNMODE(clients[fd].getNickname(), params[1]));

			}
			else if (channels[channelExist(params[1])].modeIsSet(mode[i]))
			{
				// skip the mode that already setted
				continue ;
			}
			else
			{
				if ((mode[i] == '+') && (!channels[channelExist(params[1])].modeIsSet(mode[i + 1])))
				{
					channels[channelExist(params[1])].addMode(mode[i + 1]);
					i++;
				}
				else if ((mode[i] == '-') && (channels[channelExist(params[1])].modeIsSet(mode[i + 1])))
				{
					channels[channelExist(params[1])].removeMode(mode[i + 1]);
					i++;
				}
				else if (mode[i] != '+' && mode[i] != '-')
				{
					channels[channelExist(params[1])].addMode(mode[i]);
				}
			}
		}
		sendData(fd, MODE(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), params[1], mode));
		// print the modeString
		std::cout << "modeString: " << channels[channelExist(params[1])].getModes() << std::endl;
	}
}