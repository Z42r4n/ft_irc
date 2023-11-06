/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:57:00 by zarran            #+#    #+#             */
/*   Updated: 2023/11/06 11:11:32 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

// default constructor
Channel::Channel() 
{
    this->name = "*";
    this->key = "";
    this->maxClients = 0;
}

// destructor
Channel::~Channel() 
{

}

// copy constructor
Channel::Channel(Channel const & src)
{
    *this = src;
}

// assignment operator
Channel & Channel::operator=(Channel const & src)
{
    if (this != &src)
    {
        this->name = src.name;
        this->key = src.key;
        this->maxClients = src.maxClients;
        this->clients = src.clients;
        this->operators = src.operators;
    }
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
Client Channel::getClient(size_t index) const
{
    return this->clients[index];
}

// get clients size
size_t Channel::getClientsSize(void) const
{
    return this->clients.size();
}

// list clients
std::string Channel::listClients(void) const
{
    std::string list;
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        list += this->clients[i].getNickname();
        if (i < this->clients.size() - 1)
            list += " ";
    }
    return list;
}

// check if client exist
bool Channel::clientExist(Client &client)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].getNickname() == client.getNickname())
            return true;
    }
    return false;
}
