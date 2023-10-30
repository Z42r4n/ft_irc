/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:47:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/30 18:32:14 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

Client::Client()
{
    this->isRegistered = false;
    this->isOperator = false;
    this->isGetPassword = false;
    this->nickname = "*";
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

struct sockaddr_in Client::getAddr(void) const
{
    return this->addr;
}

void Client::setAddr(struct sockaddr_in addr)
{
    this->addr = addr;
}

bool Client::isRegistered(void) const
{
    return this->isRegistered;
}

void Client::setIsRegistered(bool isRegistered)
{
    this->isRegistered = isRegistered;
}

bool Client::isOperator(void) const
{
    return this->isOperator;
}

void Client::setIsOperator(bool isOperator)
{
    this->isOperator = isOperator;
}

bool Client::isGetPassword(void) const
{
    return this->isGetPassword;
}

void Client::setIsGetPassword(bool isGetPassword)
{
    this->isGetPassword = isGetPassword;
}

std::string Client::getNickname(void) const
{
    return this->nickname;
}

void Client::setNickname(std::string name)
{
    this->nickname = name;
}
