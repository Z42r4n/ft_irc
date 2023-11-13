/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:46:39 by zarran            #+#    #+#             */
/*   Updated: 2023/11/13 15:28:36 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ircserv.hpp>

// class Client
class Client
{
    public:
        Client();
        ~Client();
        Client(Client const & src);
        Client & operator=(Client const & src);
        
        // getters
        std::string getNickname(void) const;
        std::string getUsername(void) const;
        std::string getRealname(void) const;
        struct sockaddr_in getAddr(void) const;
        bool isRegistered(void) const;
        bool isGetPassword(void) const;
        bool isReceivedMsg(void) const;
        t_fd getFd(void) const;
        size_t getChannel(size_t index) const;
        size_t getChannelsSize(void) const;
        std::string getIp(void) const;
        
        // add channel index to the vector
        void addChannel(size_t channelIndex);
        // remove channel index from the vector
        void removeChannel(size_t channelIndex);
        // // client is in channel
        bool isInChannel(size_t channelIndex);

        // setters
        void setNickname(std::string name);
        void setUsername(std::string name);
        void setRealname(std::string name);
        void setAddr(struct sockaddr_in addr);
        void setIsRegistered(bool isRegistered);
        void setIsGetPassword(bool isGetPassword);
        void setIsReceivedMsg(bool isReceivedMsg);
        void setFd(t_fd fd);
        
        
    private:
        t_fd _fd;
        std::string _nickname;
        std::string _realname;
        std::string _username;
        std::string _password;
        bool _isRegistered;
        // if the PASS command is executed successfully this well be true
        bool _hasPassword;
        bool _isReceivedMsg;
        struct sockaddr_in _addr;
        
        // store the channels index that the client joined
        std::vector<size_t> _channels;

};