/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/22 19:26:35 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ircserv.hpp>

struct sockaddr_in clientaddr;
socklen_t addr_size = sizeof(clientaddr);
char buffer[BUFFER_SIZE];

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
    // create socket
    this->createSocket();
    // bind socket
    this->bindSocket();
    // listen socket
    this->listenSocket();
    // initialize pollfd structure
    bzero(this->fds, sizeof(this->fds));
    this->fds[0].fd = this->serverfd;
    this->fds[0].events = POLLIN;
    // main loop of server, accept and receive data
    while (true)
    {
        this->acceptSockets();
        this->receiveData();
    }
}

// create socket
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

// bind socket
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

// listen socket
void Server::listenSocket(void)
{
    // start listening for the clients, here process will go in sleep mode and will wait for the incoming connection
    if (listen(this->serverfd, MAX_CLIENTS) < 0)
        throw std::runtime_error("listen() failed: " + std::string(strerror(errno)) + "\n");
    std::cout << "Server is listening on port " << this->port << std::endl;
}

// accept sockets
void Server::acceptSockets(void)
{
    if (poll(this->fds, this->nfds + 1, -1) < 0)
        throw std::runtime_error("poll() failed: " + std::string(strerror(errno)) + "\n");
        
    if (this->nfds < MAX_CLIENTS)
    {
        if (this->fds[0].revents & POLLIN)
        {
            // this->clients[this->nfds] = accept(this->serverfd, (struct sockaddr *)&clientaddr, &addr_size);
            this->clientsfd[this->nfds] = accept(this->serverfd, (struct sockaddr *)&clientaddr, &addr_size);
            this->fds[this->nfds + 1].fd = this->clientsfd[this->nfds];
            this->fds[this->nfds + 1].events = POLLIN;
            this->nfds++;
            printf("client connected\n"); // just for debug
        }
    } 
    else 
    {
        throw std::runtime_error("too many clients\n");
    }
}

// receive data
void Server::receiveData(void)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (this->clientsfd[i] > 0 && this->fds[i + 1].revents & POLLIN)
        {
            int rcv = recv(this->clientsfd[i], buffer, BUFFER_SIZE, 0);
            if (rcv > 0)
            {
                buffer[rcv] = '\0';
                printf ("%s", buffer);
            }
            
            if (rcv < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    close(this->clientsfd[i]);
                    this->clientsfd[i] = 0;
                    this->fds[i + 1].fd = 0;
                    throw std::runtime_error("recv() failed: " + std::string(strerror(errno)) + "\n");
                }
            }
            
            if (rcv == 0)
            {
                close(this->clientsfd[i]);
                this->clientsfd[i] = 0;
                this->fds[i + 1].fd = 0;
                printf("client disconnected\n"); // just for debug
            }
        }
    }
}
