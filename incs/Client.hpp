/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:46:39 by zarran            #+#    #+#             */
/*   Updated: 2023/10/31 08:34:58 by ymoutaou         ###   ########.fr       */
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
        std::string _nickname;
        std::string _realname;
        std::string _username;
        std::string _password;
        bool _isRegistered;
        bool _hasPassword;
        struct sockaddr_in _addr;
        // socklen_t addrlen;
};