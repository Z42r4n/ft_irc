/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:22:19 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/03 14:08:44 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

namespace ft 
{
	void usage(void)
    { 
        std::cerr << BOLDRED << "Error:" << RESET << " invalid arguments\n" << std::endl;
        std::cout << "Usage: " << GREEN << "./ircserv" << RESET << " [port] [password]\n" << std::endl;
        exit (EXIT_FAILURE);
    }
	
	std::vector<std::string> ft_split(std::string str, std::string delim)
	{
		std::vector<std::string> result;
		size_t pos = 0;
		std::string token;
		while ((pos = str.find(delim)) != std::string::npos)
		{
			token = str.substr(0, pos);
			result.push_back(token);
			str.erase(0, pos + delim.length());
		}
		result.push_back(str);
		return result;
	}

	std::string ft_getStr(t_params params)
	{
		std::string str = "";

		for (size_t i = 0; i < params.size(); i++)
		{
			if (params[i].find(":") != std::string::npos)
			{
				// join the rest of the string and the rest of params ignoring :
				for (size_t j = i; j < params.size(); j++)
				{
					str += params[j];
					if (j + 1 < params.size())
						str += " ";
				}
			}
		}
		return str.substr(1, str.length());
	}

	// display formatted date
	void ft_getDate(char **buf)
	{
		time_t now = time(0);
		struct tm tstruct;
		
		tstruct = *localtime(&now);
		if (*buf)
		{
			strftime(*buf, 100, "%a %b %d %Y at %X (+01)", &tstruct);	
		}
	}
}

void Server::welcomeMessage(int i, t_fd fd)
{
	sendData(fd, RPL_WELCOME(clients[i][fd].getNickname(), clients[i][fd].getUsername()));
	sendData(fd, RPL_YOURHOST(clients[i][fd].getNickname()));
	sendData(fd, RPL_CREATED(clients[i][fd].getNickname(), start_time));
	sendData(fd, RPL_MYINFO(clients[i][fd].getNickname()));
	sendData(fd, RPL_ISUPPORT(clients[i][fd].getNickname()));

	// read the motd file ircserv.motd
	std::ifstream motdFile("./srcs/motd/ircserv.motd");
	std::string line;
	
	// send the motd file line by line
	if (motdFile.is_open())
	{
		sendData(fd, RPL_MOTDSTART(clients[i][fd].getNickname()));
		while (getline(motdFile, line))
		{
			// send the motd file line by line
			sendData(fd,RPL_MOTD(clients[i][fd].getNickname(), line));
		}
		motdFile.close();
		sendData(fd, RPL_MOTDEND(clients[i][fd].getNickname()));
	}
	else
	{
		// send MOTD file is missing
		sendData(fd, ERR_NOMOTD(clients[i][fd].getNickname()));
	}
}