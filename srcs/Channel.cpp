/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:57:00 by zarran            #+#    #+#             */
/*   Updated: 2023/11/14 13:19:03 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

// default constructor
Channel::Channel() 
{
    this->name = "*";
    this->key = "";
    this->topic = "";
    this->maxClients = 0;
    this->modeString = "";
    this->creationTime = time(NULL);
    this->topicTime = 0;
    this->operators = std::vector<Client*>();
    this->clients = std::vector<Client*>();
    this->invited = std::vector<Client*>();
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
        this->topic = src.topic;
        this->maxClients = src.maxClients;
        this->clients = src.clients;
        this->operators = src.operators;
        this->modeString = src.modeString;
        this->creationTime = src.creationTime;
        this->topicTime = src.topicTime;
        this->invited = src.invited;
    }
    return *this;
}

// constructor with parameters
Channel::Channel(std::string name, std::string key, size_t maxClients)
{
    this->name = name;
    this->key = key;
    this->maxClients = maxClients;
    this->modeString = "";
    this->topic = "No topic is set";
    this->topicTime = 0;
    this->operators = std::vector<Client*>();
    this->clients = std::vector<Client*>();
    this->invited = std::vector<Client*>();
    

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

// remove client
void Channel::removeClient(Client *client)
{
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i] == client)
        {
            this->clients.erase(this->clients.begin() + i);
            break ;
        }
    }
}

// add operator
void Channel::addOperator(Client *client)
{
    this->operators.push_back(client);
}

// remove operator
void Channel::removeOperator(Client *client)
{
    for (size_t i = 0; i < this->operators.size(); i++)
    {
        if (this->operators[i] == client)
        {
            this->operators.erase(this->operators.begin() + i);
            break ;
        }
    }
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
            if (isOperator(*client))
                list += "@";
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

// remove mode
void Channel::removeMode(char mode)
{
    this->modeString.erase(this->modeString.find(mode), 1);
}

// mode alredy setted
bool Channel::modeIsSet(char mode)
{
    if (this->modeString.find(mode) != std::string::npos)
        return true;
    return false;
}

// check if client is operator
bool Channel::isOperator(Client &client) const
{
    for (size_t i = 0; i < this->operators.size(); i++)
    {
        if (this->operators[i]->getNickname() == client.getNickname())
            return true;
    }
    return false;
}

// get topic
std::string Channel::getTopic(void) const
{
    return this->topic;
}

// set topic
void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

// get topic time
time_t Channel::getTopicTime(void) const
{
    return this->topicTime;
}

// set topic time
void Channel::setTopicTime(time_t topicTime)
{
    this->topicTime = topicTime;
}

// add to invite list
void Channel::addInvited(Client *client)
{
    this->invited.push_back(client);
}

// remove from invite list
void Channel::removeInvited(Client *client)
{
    for (size_t i = 0; i < this->invited.size(); i++)
    {
        if (this->invited[i] == client)
        {
            this->invited.erase(this->invited.begin() + i);
            break ;
        }
    }
}

// client is invited
bool Channel::isInvited(Client &client) const
{
    for (size_t i = 0; i < this->invited.size(); i++)
    {
        if (this->invited[i]->getNickname() == client.getNickname())
            return true;
    }
    return false;
}
