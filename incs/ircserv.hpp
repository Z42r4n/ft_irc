/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:55:59 by zarran            #+#    #+#             */
/*   Updated: 2023/11/03 17:04:16 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRCSERV_HPP__
# define __IRCSERV_HPP__

// DEFINE IRC SERVER MESSAGES
#define ERR_UNKNOWNCOMMAND(p1, p2) ":1337.ma 421 " + p1 + " " + p2 + " :Unknown command\r\n"
#define ERR_ALREADYREGISTERED(p1) ":1337.ma 462 " + p1 + " :Connection already registered\r\n"
#define ERR_NEEDMOREPARAMS(p1, p2) ":1337.ma 461 " + p1 + " " + p2 + " :Syntax error\r\n"
#define ERR_PASSWDMISMATCH(p1) ":1337.ma 464 " + p1 + " :Invalid password\r\n"
#define ERR_NICKNAMEINUSE(p1, p2) ":1337.ma 433 " + p1 + " " + p2 + " :Nickname already in use\r\n"
#define ERR_ERRONEUSNICKNAME(p1, p2) ":1337.ma 432 " + p1 + " " + p2 + " :Erroneus nickname\r\n"
#define ERR_NICKNAMETOOLONG(p1, p2) ":1337.ma 432 " + p1 + " " + p2 + " :Nickname too long, max. 9 characters\r\n"

#define RPL_WELCOME(p1, p2) ":1337.ma 001 " + p1 + " :Welcome to the Internet Relay Network " + p1 + "!~" + p2 + "@localhost\r\n"
#define RPL_YOURHOST(p1) ":1337.ma 002 " + p1 + " :Your host is 1337.ma, running version 1.0\r\n"
#define RPL_CREATED(p1, p2) ":1337.ma 003 " + p1 + " :This server has been started " + p2 + "\r\n"
#define RPL_MYINFO(p1) ":1337.ma 004 " + p1 + " 1337.ma 1.0 o o\r\n"
#define RPL_ISUPPORT(p1) ":1337.ma 005 " + p1 + " :CHANTYPES=# :PREFIX=(ov)@+ :NETWORK=1337\r\n"
#define RPL_MOTDSTART(p1) ":1337.ma 375 " + p1 + " :- 1337.ma Message of the day - \r\n"
#define RPL_MOTD(p1, p2) ":1337.ma 372 " + p1 + " :- " + p2 + "\r\n" 
#define RPL_MOTDEND(p1) ":1337.ma 376 " + p1 + " :End of MOTD command\r\n"
#define ERR_NOMOTD(p1) ":1337.ma 422 " + p1 + " :MOTD File is missing\r\n"

// define irc error message
#define ERROR(p1) "ERROR :" + p1 + "\r\n"

// define irc notice message
#define NOTICE(p1) ":1337.ma NOTICE " + p1 + " :Closing Link: (Quit: " + p1 + ")\r\n"

// DEFINES

#define USERLEN 12
#define MAX_CLIENTS 20
#define BUFFER_SIZE 1024

// TYPEDEFS

typedef int t_fd;
typedef unsigned int t_port;

// INCLUDES

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

#include <map>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include <Colors.hpp>
#include <Server.hpp>
#include <Client.hpp>
#include <Channel.hpp>

// PROTOTYPES
namespace ft 
{
	// /srcs/utils/tools.cpp
	std::vector<std::string> ft_split(std::string str, std::string delim);
	std::string ft_getStr(t_params params);
	void ft_usage(void);
	void ft_getDate(char **buf);
}

#endif