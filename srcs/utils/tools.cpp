/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:22:19 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/15 13:24:31 by ymoutaou         ###   ########.fr       */
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

	// check modes, supported modes: i, t, k, o, l, + and -;
	bool validModes(char modes)
	{
		std::string modesList = "-+itkolsn";
		if (modesList.find(modes) != std::string::npos)
			return true;
		return false;
	} 

	// replace all spaces with one space
	std::string ft_replace(std::string str, std::string oldStr, std::string newStr)
	{
		size_t pos = 0;
		while ((pos = str.find(oldStr)) != std::string::npos)
		{
			str.replace(pos, oldStr.length(), newStr);
		}
		return str;
	}

	// conver string to upper case
	std::string ft_toupper(std::string str)
	{
		for (size_t i = 0; i < str.length(); i++)
		{
			str[i] = std::toupper(str[i]);
		}
		return str;
	}

	// check if the string is a number
	bool isNumber(std::string str)
	{
		for (size_t i = 0; i < str.length(); i++)
		{
			if (!std::isdigit(str[i]))
				return false;
		}
		return true;
	}

}

// welcome message, Server.hpp
void Server::welcomeMessage(t_fd fd)
{
	sendData(fd, RPL_WELCOME(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp()));
	sendData(fd, RPL_YOURHOST(clients[fd].getNickname()));
	sendData(fd, RPL_CREATED(clients[fd].getNickname(), start_time));
	sendData(fd, RPL_MYINFO(clients[fd].getNickname()));
	sendData(fd, RPL_ISUPPORT(clients[fd].getNickname()));

	// read the motd file ircserv.motd
	std::ifstream motdFile("./srcs/motd/ircserv.motd");
	std::string line;
	
	// send the motd file line by line
	if (motdFile.is_open() && motdFile.good())
	{
		sendData(fd, RPL_MOTDSTART(clients[fd].getNickname()));
		while (getline(motdFile, line))
		{
			// send the motd file line by line
			sendData(fd,RPL_MOTD(clients[fd].getNickname(), line));
		}
		motdFile.close();
		sendData(fd, RPL_MOTDEND(clients[fd].getNickname()));
	}
	else
	{
		// send MOTD file is missing
		sendData(fd, ERR_NOMOTD(clients[fd].getNickname()));
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
void Server::channelBroadcast(t_fd fd, std::string str, size_t channelIndex, int type, std::string msg, std::string comment)
{
	if (type == _JOIN)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			sendData(channels[channelIndex].getClient(j)->getFd(), JOIN(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), str));
		}
	}
	else if (type == _NICK)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			
			if (!channels[channelIndex].getClient(j)->isReceivedMsg())
			{
				sendData(channels[channelIndex].getClient(j)->getFd(), NICK(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), str));
				channels[channelIndex].getClient(j)->setIsReceivedMsg(true);
			}
		}
		
	}
	else if (type == _MSG)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			if (channels[channelIndex].getClient(j)->getFd() != fd)
			{
				if (!channels[channelIndex].getClient(j)->isReceivedMsg())
				{
					sendData(channels[channelIndex].getClient(j)->getFd(), PRIVMSG(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), str, msg));
					channels[channelIndex].getClient(j)->setIsReceivedMsg(true);
				}
			}
		}
	}
	else if (type == _PART)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			sendData(channels[channelIndex].getClient(j)->getFd(), PART(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), str, msg));
		}
	}
	else if (type == _TOPIC)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			sendData(channels[channelIndex].getClient(j)->getFd(), TOPIC(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), str, msg));
		}
	}
	else if (type == _KICK)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			sendData(channels[channelIndex].getClient(j)->getFd(), KICK(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), str, msg, comment));
		}
	}
	else if (type == _QUIT)
	{
		for (size_t j = 0; j < channels[channelIndex].getClientsSize(); j++)
		{
			// skip the client that send the message
			if (channels[channelIndex].getClient(j)->getFd() != fd)
			{
				if (!channels[channelIndex].getClient(j)->isReceivedMsg())
				{
					sendData(channels[channelIndex].getClient(j)->getFd(), QUIT(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), str));
					channels[channelIndex].getClient(j)->setIsReceivedMsg(true);
				}
			}
		}
	}
}
