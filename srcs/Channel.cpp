/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:57:00 by zarran            #+#    #+#             */
/*   Updated: 2023/11/08 14:57:23 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

// default constructor
Channel::Channel() 
{
    this->name = "*";
    this->key = "";
    this->maxClients = 0;
    this->modeString = "";
    this->creationTime = time(NULL);
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
        this->modeString = src.modeString;
        this->creationTime = src.creationTime;
    }
    return *this;
}

// constructor with parameters
Channel::Channel(std::string name, std::string key, size_t maxClients)
{
    this->name = name;
    this->key = key;
    this->maxClients = maxClients;

    // get current time in the timestamp unit
    this->creationTime = time(NULL);
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
void Channel::addClient(Client *client)
{
    this->clients.push_back(client);
}

// add operator
void Channel::addOperator(Client &client)
{
    this->operators.push_back(client);
}

// get clients
Client* Channel::getClient(size_t index) const
{
    return this->clients[index];
}

// get clients size
size_t Channel::getClientsSize(void) const
{
    return this->clients.size();
}

// list clients
std::string Channel::listClients() const
{
    std::string list;
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (Client* client = this->clients[i]) {
            list += client->getNickname(); 
            if (i < this->clients.size() - 1)
                list += " ";
        }
    }
    return list;
}


// check if client exist
bool Channel::clientExist(Client &client)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i]->getNickname() == client.getNickname())
            return true;
    }
    return false;
}

// get creation time
time_t Channel::getCreationTime(void) const
{
    return this->creationTime;
}

// get modes
std::string Channel::getModes(void) const
{
    return this->modeString;
}

// set modes
void Channel::setModes(std::string modes)
{
    this->modeString = modes;
}

// add mode
void Channel::addMode(char mode)
{
    this->modeString += mode;
}

// mode alredy setted
bool Channel::modeIsSet(char mode)
{
    if (this->modeString.find(mode) != std::string::npos)
        return true;
    return false;
}
