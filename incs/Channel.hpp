/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:55:56 by zarran            #+#    #+#             */
/*   Updated: 2023/10/24 15:00:28 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ircserv.hpp>

class Channel
{
    public:
        Channel();
        ~Channel();
        Channel(Channel const & src);
        Channel & operator=(Channel const & src);
        Channel(std::string name);
        
        std::string getName(void) const;
        
        void setName(std::string name);
        
    private:
        std::string name;
};