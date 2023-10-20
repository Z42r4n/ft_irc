/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:59 by zarran            #+#    #+#             */
/*   Updated: 2023/10/20 17:12:52 by zarran           ###   ########.fr       */
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

void Server::waitPoll(void)
{
    // initialize pollfd structure
    memset(this->fds, 0, sizeof(this->fds));
    this->fds[0].fd = this->serverfd;
    this->fds[0].events = POLLIN;
    // wait for poll events
    while (true)
    {
        int poll_count = poll(this->fds, MAX_CLIENTS, -1);
        if (poll_count < 0)
            throw std::runtime_error("poll() failed: " + std::string(strerror(errno)) + "\n");
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (this->fds[i].revents == 0)
                continue;
            if (this->fds[i].revents != POLLIN)
                throw std::runtime_error("revents != POLLIN\n");
            if (this->fds[i].fd == this->serverfd)
                this->acceptSocket();
            else
                throw std::runtime_error("not serverfd\n");
        }
    }
  
}

void Server::acceptSocket(void)
{
    // handle multiple clients
    
    int new_socket, valread;
    std::vector<int> clients(MAX_CLIENTS, 0);
    char buffer[BUFFER_SIZE] = {0};
    
    fds[0].fd = this->serverfd;
    fds[0].events = POLLIN;

    // Initialize the client sockets to 0
    for (int i = 1; i <= MAX_CLIENTS; i++) {
        clients[i - 1] = 0;
        fds[i].fd = 0;
        fds[i].events = POLLIN;
    }

    while (true) {
        // Wait for events on the sockets
        int activity = poll(fds, MAX_CLIENTS + 1, -1);
        if (activity < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // Check for incoming connections
        if (fds[0].revents & POLLIN) {
            if ((new_socket = accept(serverfd, (struct sockaddr *)&serv_addr, (socklen_t*)&serv_addr)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // Add the new client socket to the pollfd array
            for (int i = 1; i <= MAX_CLIENTS; i++) {
                if (clients[i - 1] == 0) {
                    clients[i - 1] = new_socket;
                    fds[i].fd = new_socket;
                    break;
                }
            }
        }

        // Check for data on the client sockets
        for (int i = 1; i <= MAX_CLIENTS; i++) {
            if (clients[i - 1] > 0 && fds[i].revents & POLLIN) {
                valread = read(clients[i - 1], buffer, BUFFER_SIZE);
                if (valread <= 0) {
                    // Client disconnected
                    close(clients[i - 1]);
                    clients[i - 1] = 0;
                    fds[i].fd = 0;
                } else {
                    // Echo the message back to the client
                    send(clients[i - 1], buffer, strlen(buffer), 0);
                }
            }
        }
    }
}
