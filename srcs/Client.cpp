/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:47:59 by zarran            #+#    #+#             */
/*   Updated: 2023/11/03 11:12:40 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

Client::Client()
{
    this->_isRegistered = false;
    this->_hasPassword = false;
    this->_nickname = "*";
    this->_realname = "";
    this->_username = "";
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
        this->_isRegistered = src._isRegistered;
        this->_nickname = src._nickname;
        this->_realname = src._realname;
        this->_username = src._username;
    }
    return *this;
}

struct sockaddr_in Client::getAddr(void) const
{
    return this->_addr;
}

void Client::setAddr(struct sockaddr_in addr)
{
    this->_addr = addr;
}

bool Client::isRegistered(void) const
{
    return this->_isRegistered;
}

void Client::setIsRegistered(bool isRegistered)
{
    this->_isRegistered = isRegistered;
}

bool Client::isGetPassword(void) const
{
    return this->_hasPassword;
}

void Client::setIsGetPassword(bool password)
{
    this->_hasPassword = password;
}

// nickname
std::string Client::getNickname(void) const
{
    return this->_nickname;
}

void Client::setNickname(std::string name)
{
    this->_nickname = name;
}

// username
std::string Client::getUsername(void) const
{
    return this->_username;
}

void Client::setUsername(std::string name)
{
    this->_username = name;
}

// realname 
std::string Client::getRealname(void) const
{
    return this->_realname;
}

void Client::setRealname(std::string name)
{
    this->_realname = name;
}