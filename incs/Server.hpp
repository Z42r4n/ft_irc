/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:48:58 by zarran            #+#    #+#             */
/*   Updated: 2023/10/16 18:54:12 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Server
{
    public:
        Server();
        ~Server();
        Server(Server const & src);
        Server & operator=(Server const & src);
    private:
        int port;
        std::string password;
};