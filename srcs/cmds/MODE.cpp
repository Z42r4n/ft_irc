/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:07:15 by houadou           #+#    #+#             */
/*   Updated: 2023/11/16 21:07:54 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

int posOfOp(std::string mode)
{
	int i = 0, p = 0;

	while (i < (int)mode.length())
	{
		if (mode[i] == '+' || mode[i] == '-' || mode[i] == 'i' || mode[i] == 't')
		{
			i++;
			continue ;
		}
		if (mode[i] == 'o')
			return (p + 3);
		p++;
		i++;
	}
	return (-1);
}

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
	if (params.size() < 2)
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
		sendData(fd, RPL_CHANNELMODEIS(clients[fd].getNickname(), params[1], "+" + channels[channelExist(params[1])].getModes()));
		sendData(fd, RPL_CREATIONTIME(clients[fd].getNickname(), params[1], std::to_string(channels[channelExist(params[1])].getCreationTime())));
		return ;
	}

	// check if the client is operator
	if (!channels[channelExist(params[1])].isOperator(clients[fd]))
	{
		// send irc server error message
		sendData(fd, ERR_CHANOPRIVSNEEDED(clients[fd].getNickname(), params[1]));
		return ;
	}

	bool flag_mode = false;

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

		// set the mode
		int pos_of_o = 0;
		for (size_t i = 0; i < mode.size(); i++) // first one is +
		{
			printf("mode[i]: %c\n", mode[i]);
			if (!ft::validModes(mode[i]))
			{
				// send irc server error message
				sendData(fd, ERR_UNKNOWNMODE(clients[fd].getNickname(), params[1]));

			}

			if (mode[i] == '+' && mode[i + 1] == 'o')
			{
				if (params.size () <= 3)
				{
					sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
					i++;
					continue ;
				}
				
				pos_of_o = posOfOp(mode);

				// chekc if the client on the server
				if (getClientFd(params[pos_of_o]) == -1)
				{
					// send irc server error message
					sendData(fd, ERR_NOSUCHNICK(clients[fd].getNickname(), params[pos_of_o]));
					i++;
					continue ;
				}
				
				// check if the client is on the channel
				else if (!channels[channelExist(params[1])].getClientByNickname(params[pos_of_o]))
				{
					// send irc server error message
					sendData(fd, ERR_USERNOTINCHANNEL(clients[fd].getNickname(), params[pos_of_o], params[1]));
					i++;
					continue ;
				}

				// check if the client is already operator
				else if (channels[channelExist(params[1])].isOperator(clients[getClientFd(params[pos_of_o])]))
				{
					i++;
					continue ;
				}

				else 
				{
					// set the client operator
					channels[channelExist(params[1])].addOperator(&(clients[getClientFd(params[pos_of_o])]));
					flag_mode = true;
					i++;
					continue;
				}
			}
				

			if (mode[i] == '-' && mode[i + 1] == 'o')
			{
				if (params.size () <= 3)
				{
					sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
					i++;
					continue ;
				}
				
				pos_of_o = posOfOp(mode);

				// chekc if the client on the server
				if (getClientFd(params[pos_of_o]) == -1)
				{
					// send irc server error message
					sendData(fd, ERR_NOSUCHNICK(clients[fd].getNickname(), params[pos_of_o]));
					i++;
					continue ;
				}
				
				// check if the client is on the channel
				else if (!channels[channelExist(params[1])].getClientByNickname(params[pos_of_o]))
				{
					// send irc server error message
					sendData(fd, ERR_USERNOTINCHANNEL(clients[fd].getNickname(), params[pos_of_o], params[1]));
					i++;
					continue ;
				}

				// check if the client is already operator
				else if (channels[channelExist(params[1])].isOperator(clients[getClientFd(params[pos_of_o])]))
				{
					channels[channelExist(params[1])].removeOperator(&(clients[getClientFd(params[pos_of_o])]));
					flag_mode = true;
					i++;
					continue ;
				}
				else
				{
					i++;
					continue;
				}
			}

			if (mode[i] == 'o')
			{
				
				if (params.size () <= 3)
				{
					sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
					continue ;
				}
				
				pos_of_o = posOfOp(mode);

				// chekc if the client on the server
				if (getClientFd(params[pos_of_o]) == -1)
				{
					// send irc server error message
					sendData(fd, ERR_NOSUCHNICK(clients[fd].getNickname(), params[pos_of_o]));
					continue ;
				}
				
				// check if the client is on the channel
				else if (!channels[channelExist(params[1])].getClientByNickname(params[pos_of_o]))
				{
					// send irc server error message
					sendData(fd, ERR_USERNOTINCHANNEL(clients[fd].getNickname(), params[pos_of_o], params[1]));
					continue ;
				}

				// check if the client is already operator
				else if (channels[channelExist(params[1])].isOperator(clients[getClientFd(params[pos_of_o])]))
				{
					continue ;
				}

				else 
				{
					// set the client operator
					channels[channelExist(params[1])].addOperator(&(clients[getClientFd(params[pos_of_o])]));
					flag_mode = true;
				}
			}

			// check if the mode string has the mode 'l' and the mode 'l' is already set and the params[3] is not empty

			int flag = 0;

			if (mode.find('l') != std::string::npos || mode.find('k') != std::string::npos || mode.find('i') != std::string::npos || mode.find('o') != std::string::npos || mode.find('t') != std::string::npos)
			{
				// reset the mode 'l'
				flag = 1;
			}

			//check if the mode is other thing than 'l' or 'k' and the mode is already set
			if (flag == 0)
			{
				// skip the mode that already setted
				continue ;
			}

		
			
			else if (flag == 1)
			{
				if (mode[i] == '+')
				{
					if (mode[i + 1] == 'l')
					{
						// find the last 'l' in the mode string
						size_t pos = std::count(mode.begin(), mode.end(), 'l');
						size_t k_count = std::count(mode.begin(), mode.end(), 'k');
						if (mode.find('k') < mode.find('l'))
							pos = pos + k_count;
						size_t o_count = std::count(mode.begin(), mode.end(), 'o');
						if (mode.find('o') < mode.find('l'))  
							pos = pos + o_count;
						if (mode.find("-l") != std::string::npos)
						{
							pos -= mode.find("-l");
							pos++;
						}
						if (pos == 0)
							pos = 1;
						if (params.size() >= 4)
						{
		  					size_t l = 0;
							for (size_t j = 0; j <= (params.size() - 3); j++)
							{
								// check if there is only one limit in the mode string and the params all of them are intergers , we need to take the last one
								if (pos > params.size() - 3)
								{
									size_t m = pos - (params.size() - 3);
									pos = pos - m;
								}
								if (pos == 1)
								{
									for (size_t k = 0; k <= (params.size() - 3); k++)
									{
										if (ft::isNumber(params[k + 2]))
										{
											if ((int)channels[channelExist(params[1])].getClientsSize() < std::atoi((params[k + 2]).c_str()))
											{
												channels[channelExist(params[1])].setLimit(std::atoi((params[k + 2]).c_str()));
												flag_mode = true;
												break;
											}
										}
									}
								}
								else if (j == pos && pos != 1)
								{
									l = j;
									if (ft::isNumber(params[j + 2]))
									{
										channels[channelExist(params[1])].setLimit(std::atoi((params[j + 2]).c_str()));
										flag_mode = true;
									}
									else
									{
										sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
										continue ;
									}
								}
							}
							if (l != pos && pos != 1 && channels[channelExist(params[1])].getLimit() == 0)
							{
								sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
								i++;
								continue ;
							}
							if (channels[channelExist(params[1])].getLimit() == 0)
							{
								i++;
								continue ;
							}
						}
						else
						{
							sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
							continue ;
						}
					}
					else if (mode[i + 1] == 'k')
					{
						size_t pos = std::count(mode.begin(), mode.end(), 'k');
						size_t k_count = std::count(mode.begin(), mode.end(), 'l');
						if (mode.find('l') < mode.find('k'))
							pos = pos + k_count;
						size_t o_count = std::count(mode.begin(), mode.end(), 'o');
						if (mode.find('o') < mode.find('k'))
							pos = pos + o_count;
						if (mode.find("-k") != std::string::npos)
						{
							pos -= mode.find("-k");
							pos++;
						}
						if (pos == 0)
							pos = 1;
						if (params.size() >= 4)
						{
							size_t l = 0;
							for (size_t j = 0; j <= (params.size() - 3); j++)
							{
								// check if there is only one limit in the mode string and the params all of them are intergers , we need to take the last one
								if (pos > params.size() - 3)
								{
									size_t m = pos - (params.size() - 3);
									pos = pos - m;
								}
								if (pos == 1)
								{
									for (size_t k = 1; k <= (params.size() - 3); k++)
									{
										channels[channelExist(params[1])].setKey(params[k + 2]);
										flag_mode = true;
										break;
									}
								}
								else if (j == pos && pos != 1)
								{
									l = j;
									channels[channelExist(params[1])].setKey(params[j + 2]);
									flag_mode = true;
								}
							}
							if (l != pos && pos != 1 && channels[channelExist(params[1])].getKey() == "")
							{
								sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
								i++;
								continue ;
							}
						}
						else
						{
							sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
							continue ;
						}
					}
					if (channels[channelExist(params[1])].getModes().find(mode[i + 1]) == std::string::npos)
					{
						// check if the mode is in modestring
						channels[channelExist(params[1])].addMode(mode[i + 1]);
						flag_mode = true;
					}
					i++;
				}
				else if (mode[i] == '-')
				{
					// remove the mode from the modestring
					if (mode[i + 1] == 'l')
					{
						channels[channelExist(params[1])].setLimit(0);
					}
					if (mode[i + 1] == 'k')
					{
						channels[channelExist(params[1])].setKey("");
					}
					if (channels[channelExist(params[1])].getModes().find(mode[i + 1]) != std::string::npos)
					{
						// check if the mode is in modestring
						channels[channelExist(params[1])].removeMode(mode[i + 1]);
						flag_mode = true;
					}
					i++;
				}
				else if (mode[i] != '+' && mode[i] != '-' && mode[i - 1] != '+' && mode[i - 1] != '-')
				{
					if (mode[i] == 'l')
					{
						// find the last 'l' in the mode string
						size_t pos = std::count(mode.begin(), mode.end(), 'l');
						size_t k_count = std::count(mode.begin(), mode.end(), 'k');
						if (mode.find('k') < mode.rfind('l'))
							pos = pos + k_count;
						size_t o_count = std::count(mode.begin(), mode.end(), 'o');
						if (mode.find('o') < mode.rfind('l'))
							pos = pos + o_count;
						if (mode.find("-l") != std::string::npos)
						{
							pos -= mode.find("-l") + 1;
						}
						if (pos == 0)
							pos = 1;
							std::cout << "pos: " << pos << std::endl;
						if (params.size() >= 4)
						{
							// check if 't' or 'i' are setted before 'l'
							size_t l = 0;
							for (size_t j = 0; j <= (params.size() - 3); j++)
							{
								if (pos > params.size() - 3)
								{
									size_t m = pos - (params.size() - 3);
									pos = pos - m;
								}
								// check if there is only one limit in the mode string and the params all of them are intergers , we need to take the last one
								if (pos == 1)
								{
									for (size_t k = 0; k <= (params.size() - 3); k++)
									{
										if (ft::isNumber(params[k + 2]))
										{
											if ((int)channels[channelExist(params[1])].getClientsSize() < std::atoi((params[k + 2]).c_str()))
											{
												channels[channelExist(params[1])].setLimit(std::atoi((params[k + 2]).c_str()));
												break;
											}
										}
									}
								}
								else if (j == pos && pos != 1)
								{
									l = j;
									if (ft::isNumber(params[j + 2]))
									{
										channels[channelExist(params[1])].setLimit(std::atoi((params[j + 2]).c_str()));
									}
									else
									{
										sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
										continue ;
									}
								}
							}
							if (l != pos && pos != 1 && channels[channelExist(params[1])].getLimit() == 0)
							{
								sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
								// i++;
								continue ;
							}
							if (channels[channelExist(params[1])].getLimit() == 0)
							{
								continue ;
							}
						}
						else
						{
							sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
							continue ;
						}
					}
					else if (mode[i] == 'k')
					{
						size_t pos = std::count(mode.begin(), mode.end(), 'k');
						size_t l_count = std::count(mode.begin(), mode.end(), 'l');
						if (mode.find('l') < mode.rfind('k'))
							pos = pos + l_count;
						size_t o_count = std::count(mode.begin(), mode.end(), 'o');
						if (mode.find('o') < mode.rfind('k'))
							pos = pos + o_count;
						if (mode.find("-k") != std::string::npos)
						{
							pos -= mode.find("-k") + 1;
						}
						if (pos == 0)
							pos = 1;
							std::cout << "pos: " << pos << std::endl;
						if (params.size() >= 4)
						{
							size_t l = 0;
							for (size_t j = 0; j <= (params.size() - 3); j++)
							{
								// check if there is only one limit in the mode string and the params all of them are intergers , we need to take the last one
								if (pos > params.size() - 3)
								{
									size_t m = pos - (params.size() - 3);
									pos = pos - m;
								}
								std::cout << "pos k: " << pos << std::endl;
								if (pos == 1)
								{
									for (size_t k = 1; k <= (params.size() - 3); k++)
									{
										channels[channelExist(params[1])].setKey(params[k + 2]);
										flag_mode = true;
										break;
									}
								}
								else if (j == pos && pos != 1)
								{
									l = j;
									channels[channelExist(params[1])].setKey(params[j + 2]);
									flag_mode = true;
								}
							}
							if (l != pos && pos != 1 && channels[channelExist(params[1])].getKey() == "")
							{
								sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
								continue ;
							}
						}
						else
						{
							sendData(fd, ERR_NEEDMOREPARAMS(clients[fd].getNickname(), params[0]));
							continue ;
						}
					}
					if (channels[channelExist(params[1])].getModes().find(mode[i]) == std::string::npos)
					{
						// check if the mode is in modestring
						channels[channelExist(params[1])].addMode(mode[i]);
						flag_mode = true;
					}
				}
			}
		}
		
		size_t f = 0;
		if (channels[channelExist(params[1])].modeIsSet('l') && channels[channelExist(params[1])].getLimit() != 0)
			mode += " limit: " + std::to_string(channels[channelExist(params[1])].getLimit());
		if (channels[channelExist(params[1])].modeIsSet('k') && mode.find('k') != std::string::npos)
			mode += " key: " + channels[channelExist(params[1])].getKey();
		if ((mode.find('o') != std::string::npos || mode.find("+o") != std::string::npos) && pos_of_o != 0 && getClientFd(params[pos_of_o]) != -1 && channels[channelExist(params[1])].isOperator(clients[getClientFd(params[pos_of_o])]))
		{
			mode += " new op: " + params[pos_of_o];
			f = 1;
		}
		if (mode.find("-o") != std::string::npos && pos_of_o != 0 && f == 0)
			mode += " removed op: " + params[pos_of_o];
		
		if (flag_mode == true)
		{
			// sendData(fd, MODE(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), params[1], mode));
			for (size_t j = 0; j < channels[channelExist(params[1])].getClientsSize(); j++)
			{
				sendData(channels[channelExist(params[1])].getClient(j)->getFd(), MODE(clients[fd].getNickname(), clients[fd].getUsername(), clients[fd].getIp(), params[1],  mode));
			}
			flag_mode = false;
		}
		// print the modeString
		(void)flag_mode;
		std::cout << "modeString: " << channels[channelExist(params[1])].getModes() << std::endl;
	}
	std::cout << "here\n";
}