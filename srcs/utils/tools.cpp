/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:22:19 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/06 11:47:43 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

namespace ft 
{
	// the usage message
	void ft_usage(void)
    { 
        std::cerr << BOLDRED << "Error:" << RESET << " invalid arguments\n" << std::endl;
        std::cout << "Usage: " << GREEN << "./ircserv" << RESET << " [port] [password]\n" << std::endl;
        exit (EXIT_FAILURE);
    }

	// check if the nickname is valid
	bool isNicknameValid(std::string nickname)
	{
		std::string chars = "[]\\`_^{|}";
		
		// check if nickname is valid
		for (size_t i = 0; i < nickname.length(); i++)
		{
			//check if the nickname not start with acceptable chars
			if (i == 0 && nickname[i] != '_' && !std::isalpha(nickname[i]))
			{
				return false;
			}
			
			// check if the nickname contains only acceptable chars
			if (!std::isalnum(nickname[i]) && chars.find(nickname[i]) == std::string::npos)
			{
				return false;
			}
		}
		
		return true;
	}
	
	// split string by delim
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

	// get the string after the first :
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

	// get the string from the index
	std::string ft_getStrFromIndex(int index, t_params params)
	{
		std::string str = "";

		for (int i = index; i < (int)params.size(); i++)
		{
			str += params[i];
			if (i != (int)params.size() - 1)
				str += "+";
		}
		return str;
	}
}

// welcome message, Server.hpp
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
	if (motdFile.is_open() && motdFile.good())
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

// check if the channel exist and return the index of the channel
int Server::channelExist(std::string channelName)
{
	// check if the channel exist
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == channelName)
			return i;
	}
	return -1;
}

// broadcast message to all clients in channel
void Server::channelBroadcast(int i, t_fd fd, t_params params, size_t channelIndex, int type)
{
	if (type == _JOIN)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			sendData(channels[channelIndex].getClient(j).getFd(), JOIN(clients[i][fd].getNickname(), clients[i][fd].getUsername(), params[1]));
		}
	}
	else if (type == _NICK)
	{
		//print the size of the clients in the channel
		std::cout << channels[channelIndex].getClientsSize() << std::endl;
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			sendData(channels[channelIndex].getClient(j).getFd(), NICK(clients[i][fd].getNickname(), clients[i][fd].getUsername(), params[1]));
		}
	}
	// else if (type == _QUIT)
	// {
	// 	for (size_t j = 0; j < channels[channelIndex].getClients().size(); j++)
	// 	{
	// 		sendData(channels[channelIndex].getClients()[j].getFd(), QUIT(clients[i][fd].getNickname(), ft_getStr(params)));
	// 	}
	// }
	// else if (type == _PART)
	// {
	// 	for (size_t j = 0; j < channels[channelIndex].getClients().size(); j++)
	// 	{
	// 		sendData(channels[channelIndex].getClients()[j].getFd(), PART(clients[i][fd].getNickname(), params[1]));
	// 	}
	// }
}
