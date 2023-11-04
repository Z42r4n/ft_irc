/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:57:00 by zarran            #+#    #+#             */
/*   Updated: 2023/11/04 15:21:38 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(Channel const & src)
{
    *this = src;
}

Channel & Channel::operator=(Channel const & src)
{
    (void)src;
    return *this;
}

Channel::Channel(std::string name)
{
    this->name = name;
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
std::string Channel::getPassword(void) const
{
    return this->password;
}

//get password
void Channel::setPassword(std::string password)
{
    this->password = password;
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
