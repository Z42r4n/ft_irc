/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 14:11:47 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/03 16:39:59 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ircserv.hpp>

void Server::quitCommand(int i, t_fd fd, t_params params)
{
	std::string quit_message = "Closing connection";
	
	// check if invalid number of params
	if (params.size() > 2 && params[1].find(":") == std::string::npos)
	{
		// send irc server error message
		sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), params[0]));
		return ;
	}
	
	// get the message after the colon symbol
	if (params[1].find(":") != std::string::npos)
	{
		// print params[1] length
		if (params[1].length() > 1)
			quit_message = ft::ft_getStr(params);
	}
	else if (params.size() == 2)
	{
		// get the message
		quit_message = params[1];
	}

	// if client is registered send notice message
	if (clients[i][fd].isRegistered())
	{
		// send notice message to client
		sendData(fd, NOTICE(clients[i][fd].getNickname()));
	}
	
	// send irc server error message
	sendData(fd, ERROR(std::string(quit_message)));
	
	// close the connection
	fds[i + 1].fd = 0;
	clients[i][fd] = Client();
	std::cout << RED << "[-] Client disconnected, socket fd is: " << RESET << YELLOW << fd << RESET << "\n" << std::endl;
	close(fd);
}