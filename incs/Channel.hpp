/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:55:56 by zarran            #+#    #+#             */
/*   Updated: 2023/11/05 08:12:06 by ymoutaou         ###   ########.fr       */
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
        std::vector<Client> getClients(void) const;
        
   
        void setName(std::string name);
        void setKey(std::string password);

        // add client
        void addClient(Client &client);
        // add operator
        void addOperator(Client &client);
        
    private:
        std::string name;
        std::string key;
        size_t maxClients;
        std::vector<Client> clients;
        std::vector<Client> operators;
};