/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:55:56 by zarran            #+#    #+#             */
/*   Updated: 2023/11/16 10:42:59 by ymoutaou         ###   ########.fr       */
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
        Channel(std::string name, std::string key = "", size_t maxClients = 0);
        
        std::string getName(void) const;
        std::string getKey(void) const;
        std::string listClients(void) const;
        Client *getClient(size_t index) const;
        size_t getClientsSize(void) const;
        time_t getCreationTime(void) const;
        std::string getModes(void) const;
        std::string getTopic(void) const;
        time_t getTopicTime(void) const;
        size_t getLimit(void) const;
        
        void setName(std::string name);
        void setKey(std::string password);
        void setLimit(size_t limit);
        void setModes(std::string modes);
        void setTopic(std::string topic);
        void setTopicTime(time_t topicTime);

        // add client
        void addClient(Client *client);
        // remove client
        void removeClient(Client *client);
        // add operator
        void addOperator(Client *client);
        // remove operator
        void removeOperator(Client *client);
        // add to invite list
        void addInvited(Client *client);
        // remove from invite list
        void removeInvited(Client *client);
        // check if client exist
        bool clientExist(Client &client);
        // add mode
        void addMode(char mode);
        // remove mode
        void removeMode(char mode);
        // mode alredy setted
        bool modeIsSet(char mode);
        // check if client is operator
        bool isOperator(Client &client) const;
        // client is invited
        bool isInvited(Client &client) const;
        // get clirn by nickname
        Client *getClientByNickname(std::string nickname);
        
    private:
        std::string name;
        std::string key;
        std::string topic;
        time_t creationTime;
        time_t topicTime;
        size_t limit;
        std::string modeString;
        std::vector<Client *> clients;
        std::vector<Client *> operators;
        std::vector<Client *> invited;
};