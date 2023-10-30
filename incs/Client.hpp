/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:46:39 by zarran            #+#    #+#             */
/*   Updated: 2023/10/30 18:27:43 by ymoutaou         ###   ########.fr       */
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
        bool isRegistered(void) const;
        bool isGetPassword(void) const;
        bool isOperator(void) const;
        // socklen_t getAddrLen(void) const;
        
        void setNickname(std::string name);
        void setAddr(struct sockaddr_in addr);
        void setIsRegistered(bool isRegistered);
        void setIsGetPassword(bool isGetPassword);
        void setIsOperator(bool isOperator);
        
    private:
        std::string nickname;
        std::string realname;
        std::string username;
        std::string password;
        bool isRegistered;
        bool isOperator;
        bool isGetPassword;
        struct sockaddr_in addr;
        // socklen_t addrlen;
};