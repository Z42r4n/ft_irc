/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/22 14:01:32 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ircserv.hpp>

Server::Server() {}

Server::~Server() {}

Server::Server(t_port port, std::string password)
{
    if (port < 6660 || port > 6669)
    {
        throw std::invalid_argument("port must be in range 6660-6669\n");   
    }
    // chaeck password, accept only printable chars
    for (size_t i = 0; i < password.length(); i++)
    {
        if (!std::isprint(password[i]))
        {
            throw std::invalid_argument("password must be printable\n");
        }
    }
    this->port = port;
    this->password = password;
    this->nfds = 0;
}


t_port Server::getPort(void) const
{
    return this->port;
}

void Server::run(void)
{
    this->createSocket();
    this->bindSocket();
    this->listenSocket();
    // this->waitPoll();
    this->acceptSocket();
}

void Server::createSocket(void)
{
    // call socket function
    if ((this->serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("socket() failed: " + std::string(strerror(errno)) + "\n");
    // set socket options
    int opt = 1;
    if (setsockopt(this->serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt() failed: " + std::string(strerror(errno)) + "\n");
}

void Server::bindSocket(void)
{
    // initialize socket structure
    bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(this->port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // bind the host address using bind() call
    if (bind(this->serverfd, (struct sockaddr *) &serv_addr, sizeof(this->serv_addr)) < 0)
        throw std::runtime_error("bind() failed: " + std::string(strerror(errno)) + "\n");
}

void Server::listenSocket(void)
{
    // start listening for the clients, here process will go in sleep mode and will wait for the incoming connection
    if (listen(this->serverfd, MAX_CLIENTS) < 0)
        throw std::runtime_error("listen() failed: " + std::string(strerror(errno)) + "\n");
    std::cout << "Server is listening on port " << this->port << std::endl;
}

void Server::acceptSocket(void)
{
    int clientfd[MAX_CLIENTS];
    struct sockaddr_in clientaddr;
    socklen_t addr_size = sizeof(clientaddr);
    char buffer[BUFFER_SIZE];

    bzero(this->fds, sizeof(this->fds));
    this->fds[0].fd = this->serverfd;
    this->fds[0].events = POLLIN;

    while (true)
    {
        if (poll(this->fds, this->nfds + 1, -1) < 0)
            throw std::runtime_error("poll() failed: " + std::string(strerror(errno)) + "\n");
            
        if (this->nfds < MAX_CLIENTS)
        {
            if (this->fds[0].revents & POLLIN)
            {
                clientfd[this->nfds] = accept(this->serverfd, (struct sockaddr *)&clientaddr, &addr_size);
                this->fds[this->nfds + 1].fd = clientfd[this->nfds];
                this->fds[this->nfds + 1].events = POLLIN;
                this->nfds++;
                printf("client connected\n");
            }
        } 
        else 
        {
            throw std::runtime_error("too many clients\n");
        }
        
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clientfd[i] > 0 && this->fds[i + 1].revents & POLLIN)
            {
                int rcv = recv(clientfd[i], buffer, BUFFER_SIZE, 0);
                if (rcv > 0)
                {
                    buffer[rcv] = '\0';
                    printf ("%s", buffer);
                }
                
                if (rcv <= 0)
                {
                   close(clientfd[i]);
                   clientfd[i] = 0;
                   this->fds[i + 1].fd = 0;
                   printf("client disconnected\n");
                }
            }
        }
    }
}
