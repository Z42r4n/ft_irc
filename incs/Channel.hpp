/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 14:55:56 by zarran            #+#    #+#             */
/*   Updated: 2023/11/04 15:21:51 by ymoutaou         ###   ########.fr       */
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
        std::string getPassword(void) const;
        std::vector<Client> getClients(void) const;
        
   
        void setName(std::string name);
        void setPassword(std::string password);

        // add client
        void addClient(Client &client);
        // add operator
        void addOperator(Client &client);
        
    private:
        std::string name;
        std::string password;
        std::vector<Client> clients;
        std::vector<Client> operators;
};