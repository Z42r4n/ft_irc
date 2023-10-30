/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:47:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/30 11:36:03 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

Client::Client()
{
    this->isRegistered = false;
    this->nickname = "";
    this->realname = "";
    this->username = "";
}

Client::~Client() {}

Client::Client(Client const & src)
{
    *this = src;
}

Client & Client::operator=(Client const & src)
{
    if (this != &src)
    {
        this->isRegistered = src.isRegistered;
        this->nickname = src.nickname;
        this->realname = src.realname;
        this->username = src.username;
    }
    return *this;
}


// t_fd Client::getFd(void) const
// {
//     return this->fd;
// }

std::string Client::getNickname(void) const
{
    return this->nickname;
}

// void Client::setFd(t_fd fd)
// {
//     this->fd = fd;
// }

void Client::setNickname(std::string name)
{
    this->nickname = name;
}
