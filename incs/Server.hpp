/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:48:58 by zarran            #+#    #+#             */
/*   Updated: 2023/11/06 12:47:22 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ircserv.hpp>

// forward declarations
class Client;
class Channel;

// typedefs
typedef std::map<t_fd, Client>::iterator t_clients_it;
typedef std::vector<Channel>::iterator t_channels_it;
typedef std::vector<std::string> t_params;

// class Server
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

        // irc replies functions
        void welcomeMessage(int i, t_fd fd);

        // check channel exist
        int channelExist(std::string channelName);

        // broadcast message to all clients in channel
        void channelBroadcast(int i, t_fd fd, t_params params, size_t channelIndex, int type);

        // commands functions
        void passCommand(int i, t_fd fd, t_params params);
        void nickCommand(int i, t_fd fd, t_params params);
        void userCommand(int i, t_fd fd, t_params params);
        void quitCommand(int i, t_fd fd, t_params params);
        void bimoCommand(int i, t_fd fd, t_params params);
        void joinCommand(int i, t_fd fd, t_params params);
        void modeCommand(int i, t_fd fd, t_params params);

    private:
        t_fd serverfd;
        t_port port;
        std::string password;
        char *start_time;
        struct sockaddr_in serv_addr;
        struct pollfd fds[MAX_CLIENTS + 1];
        int nfds;
        std::map<t_fd, Client> clients[MAX_CLIENTS];
        std::vector<Channel> channels;
        int nbChannels;
        char buffer[BUFFER_SIZE];

}; 