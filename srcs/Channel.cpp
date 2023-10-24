/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:57:00 by zarran            #+#    #+#             */
/*   Updated: 2023/10/24 14:58:39 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(std::string name)
{
    this->name = name;
}

std::string Channel::getName(void) const
{
    return this->name;
}

void Channel::setName(std::string name)
{
    this->name = name;
}

/* ************************************************************************** */
