/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:55:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/31 13:27:46 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRCSERV_HPP__
# define __IRCSERV_HPP__

// DEFINE IRC SERVER MESSAGES
#define ERR_UNKNOWNCOMMAND(p1, p2) ":irc.example.com 421 " + p1 + " " + p2 + " :Unknown command\r\n"
#define ERR_ALREADYREGISTERED(p1) ":irc.example.com 462 " + p1 + " :Connection already registered\r\n"
#define ERR_NEEDMOREPARAMS(p1, p2) ":irc.example.com 461 " + p1 + " " + p2 + " :Syntax error\r\n"
#define ERR_PASSWDMISMATCH ":irc.example.com 464 :Invalid password\r\n"

// DEFINES

#define MAX_CLIENTS 20
#define BUFFER_SIZE 1024

// TYPEDEFS

typedef int t_fd;
typedef unsigned int t_port;

// INCLUDES

#include <iostream>
#include <string>
#include <map>

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


#endif