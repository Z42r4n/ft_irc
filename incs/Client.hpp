/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:46:39 by zarran            #+#    #+#             */
/*   Updated: 2023/10/30 10:13:10 by ymoutaou         ###   ########.fr       */
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
        
        t_fd getFd(void) const;
        std::string getNickname(void) const;
        
        void setFd(t_fd fd);
        void setNickname(std::string name);
        
    private:
        // t_fd fd;
        std::string nickname;
        std::string realname;
        std::string username;
        std::string password;
        bool isRegistered;
        struct sockaddr_in addr;
        socklen_t addrlen;
};