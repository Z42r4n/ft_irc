/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:55:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/22 14:05:15 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IRCSERV_HPP__
# define __IRCSERV_HPP__

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
#include <unistd.h>
#include <poll.h>

#include <Colors.hpp>
#include <Server.hpp>
#include <Client.hpp>


#endif