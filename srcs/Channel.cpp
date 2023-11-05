/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:57:00 by zarran            #+#    #+#             */
/*   Updated: 2023/11/05 08:17:35 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

// default constructor
Channel::Channel() 
{
    this->name = "";
    this->key = "";
    this->maxClients = 0;
}

// destructor
Channel::~Channel() {}

// copy constructor
Channel::Channel(Channel const & src)
{
    *this = src;
}

// assignment operator
Channel & Channel::operator=(Channel const & src)
{
    (void)src;
    return *this;
}

// constructor with parameters
Channel::Channel(std::string name, std::string key, size_t maxClients)
{
    this->name = name;
    this->key = key;
    this->maxClients = maxClients;
}

// get name
std::string Channel::getName(void) const
{
    return this->name;
}

// get name
void Channel::setName(std::string name)
{
    this->name = name;
}

// get password
std::string Channel::getKey(void) const
{
    return this->key;
}

//get password
void Channel::setKey(std::string key)
{
    this->key = key;
}

// add client
void Channel::addClient(Client &client)
{
    this->clients.push_back(client);
}

// add operator
void Channel::addOperator(Client &client)
{
    this->operators.push_back(client);
}

// get clients
std::vector<Client> Channel::getClients(void) const
{
    return this->clients;
}
