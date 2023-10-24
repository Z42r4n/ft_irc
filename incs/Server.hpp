/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:48:58 by zarran            #+#    #+#             */
/*   Updated: 2023/10/24 15:42:15 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ircserv.hpp>

class Client;
class Server
{
    public:
        Server();
        ~Server();
        Server(Server const & src);
        Server & operator=(Server const & src);
        Server(t_port port, std::string password);
        
        t_port getPort(void) const;
        
        void run(void);
        void createSocket(void);
        void bindSocket(void);
        void listenSocket(void);
        void acceptSockets(void);
        void receiveData(void);
        void parseData(std::string data);
        void sendData(t_fd fd, std::string data);
        
    private:
        t_fd serverfd;
        t_fd clientsfd[MAX_CLIENTS];
        t_port port;
        std::string password;
        struct sockaddr_in serv_addr;
        struct pollfd fds[MAX_CLIENTS + 1];
        int nfds;
        std::map<t_fd, Client> clients[MAX_CLIENTS];
}; 