/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:55:56 by zarran            #+#    #+#             */
/*   Updated: 2023/11/09 17:22:09 by ymoutaou         ###   ########.fr       */
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
        
   
        void setName(std::string name);
        void setKey(std::string password);
        void setModes(std::string modes);

        // add client
        void addClient(Client *client);
        // add operator
        void addOperator(Client &client);
        // check if client exist
        bool clientExist(Client &client);
        // add mode
        void addMode(char mode);
        // remove mode
        void removeMode(char mode);
        // mode alredy setted
        bool modeIsSet(char mode);
        
    private:
        std::string name;
        std::string key;
        time_t creationTime;
        size_t maxClients;
        std::string modeString;
        std::vector<Client *> clients;
        std::vector<Client> operators;
};