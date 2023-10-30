/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:48:58 by zarran            #+#    #+#             */
/*   Updated: 2023/10/30 14:53:30 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ircserv.hpp>

class Client;

typedef std::map<t_fd, Client>::iterator t_clients_it;

class Server
{
    public:
        Server();
        ~Server();
        Server(Server const & src);
        Server & operator=(Server const & src);
        Server(t_port port, std::string password);
        
        t_port getPort(void) const;
        
        // server functions
        void run(void);
        void createSocket(void);
        void bindSocket(void);
        void listenSocket(void);
        void acceptSockets(void);
        void receiveData(void);
        void parseData(int i, t_fd fd, std::string data);
        void sendData(t_fd fd, std::string data);

        // commands functions
        void passCommand(int i, t_fd fd, std::string param);
        void nickCommand(int i, t_fd fd, std::string param);
        void userCommand(int i, t_fd fd, std::string param);

    private:
        t_fd serverfd;
        t_port port;
        std::string password;
        struct sockaddr_in serv_addr;
        struct pollfd fds[MAX_CLIENTS + 1];
        int nfds;
        std::map<t_fd, Client> clients[MAX_CLIENTS];
        char buffer[BUFFER_SIZE];
}; 