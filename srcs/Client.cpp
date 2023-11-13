/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:47:59 by zarran            #+#    #+#             */
/*   Updated: 2023/11/13 15:28:55 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

// default constructor

Client::Client()
{
    this->_fd = -1;
    this->_isRegistered = false;
    this->_hasPassword = false;
    this->_isReceivedMsg = false;
    this->_nickname = "*";
    this->_realname = "";
    this->_username = "";
    this->_channels = std::vector<size_t>();
    bzero(&this->_addr, sizeof(this->_addr));
}

// destructor
Client::~Client() 
{

}

// copy constructor
Client::Client(Client const & src)
{
    *this = src;
}

// assignment operator
Client & Client::operator=(Client const & src)
{
    if (this != &src)
    {
        this->_fd = src._fd;
        this->_isRegistered = src._isRegistered;
        this->_hasPassword = src._hasPassword;
        this->_isReceivedMsg = src._isReceivedMsg;
        this->_nickname = src._nickname;
        this->_realname = src._realname;
        this->_username = src._username;
        this->_channels = src._channels;
        this->_addr = src._addr;
    }
    return *this;
}

// address
struct sockaddr_in Client::getAddr(void) const
{
    return this->_addr;
}

void Client::setAddr(struct sockaddr_in addr)
{
    this->_addr = addr;
}

// registered
bool Client::isRegistered(void) const
{
    return this->_isRegistered;
}

void Client::setIsRegistered(bool isRegistered)
{
    this->_isRegistered = isRegistered;
}

// password
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

// fd
t_fd Client::getFd(void) const
{
    return this->_fd;
}

void Client::setFd(t_fd fd)
{
    this->_fd = fd;
}

// add channel name to the vector
void Client::addChannel(size_t channelIndex)
{
    this->_channels.push_back(channelIndex);
}
// remove channel name from the vector
void Client::removeChannel(size_t channelIndex)
{
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i] == channelIndex)
        {
            this->_channels.erase(this->_channels.begin() + i);
            break ;
        }
    }
}

// get channel by index
size_t Client::getChannel(size_t index) const
{
        return this->_channels[index];
}

// get channels size
size_t Client::getChannelsSize(void) const
{
    return this->_channels.size();
}

// isReceivedNickMsg
bool  Client::isReceivedMsg(void) const
{
    return this->_isReceivedMsg;
}

// set isReceivedNickMsg
void Client::setIsReceivedMsg(bool isReceivedMsg)
{
    this->_isReceivedMsg = isReceivedMsg;
}

// client is in channel
bool Client::isInChannel(size_t channelIndex)
{
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i] == channelIndex)
            return true;
    }
    return false;
}

// get clinet ip address
std::string Client::getIp(void) const
{
    return std::string(inet_ntoa(this->_addr.sin_addr));
}