/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:46:39 by zarran            #+#    #+#             */
/*   Updated: 2023/10/30 15:00:04 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ircserv.hpp>

class Client
{
    public:
        Client();
        ~Client();
        Client(Client const & src);
        Client & operator=(Client const & src);
        
        std::string getNickname(void) const;
        struct sockaddr_in getAddr(void) const;
        bool getIsRegistered(void) const;
        // socklen_t getAddrLen(void) const;
        
        void setNickname(std::string name);
        void setAddr(struct sockaddr_in addr);
        void setIsRegistered(bool isRegistered);
        
    private:
        std::string nickname;
        std::string realname;
        std::string username;
        std::string password;
        bool isRegistered;
        struct sockaddr_in addr;
        // socklen_t addrlen;
};