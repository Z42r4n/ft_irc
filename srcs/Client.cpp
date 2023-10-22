/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:47:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/22 19:12:52 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

Client::Client() {}

Client::~Client() {}

Client::Client(t_fd fd, std::string name)
{
    this->fd = fd;
    this->nickname = name;
}

t_fd Client::getFd(void) const
{
    return this->fd;
}

std::string Client::getNickname(void) const
{
    return this->nickname;
}

void Client::setFd(t_fd fd)
{
    this->fd = fd;
}

void Client::setNickname(std::string name)
{
    this->nickname = name;
}