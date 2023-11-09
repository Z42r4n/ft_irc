/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:21:59 by zarran            #+#    #+#             */
/*   Updated: 2023/11/09 14:01:22 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ircserv.hpp>

// default constructor
Server::Server() {}

// destructor
Server::~Server() 
{
	// free start_time variable
	free(start_time);
}

// copy constructor
Server::Server(Server const & src)
{
	*this = src;
}

// assignment operator
Server & Server::operator=(Server const & src)
{
	if (this != &src)
	{
		this->serverfd = src.serverfd;
		this->port = src.port;
		this->password = src.password;
		this->serverfd = src.serverfd;
		this->serv_addr = src.serv_addr;
		this->nfds = src.nfds;
		this->nbChannels = src.nbChannels;
		this->clients = src.clients;
		this->start_time = src.start_time;
	}
	return *this;
}

// constructor with parameters
Server::Server(t_port port, std::string password)
{
	// check if port is in range 6660-6669
	if (port < 6660 || port > 6669)
	{
		throw std::invalid_argument("port must be in range 6660-6669\n");   
	}
	
	// check if password is empty
	if (password.empty())
	{
		throw std::invalid_argument("password must not be empty\n");
	}
	
	// check password, accept only printable chars
	for (size_t i = 0; i < password.length(); i++)
	{
		if (!std::isprint(password[i]))
		{
			throw std::invalid_argument("password must be printable\n");
		}
	}
	
	// allocate start_time variable
	start_time = (char *)std::malloc(sizeof(char) * 100);
	
	// set port and password 
	this->port = port;
	this->password = password;
	
	// initialize fds and nbChannels
	this->nfds = 0;
	this->nbChannels = 0;
}

// get port
t_port Server::getPort(void) const
{
	return this->port;
}

// run the server
void Server::run(void)
{
	// create socket
	createSocket();
	
	// bind socket
	bindSocket();

	// get current time
	ft::ft_getDate(&start_time);

	// listen socket
	listenSocket();
	
	// initialize pollfd structure
	bzero(fds, sizeof(fds));
	fds[0].fd = serverfd;
	fds[0].events = POLLIN;
	
	// main loop of server, accept and receive data
	while (true)
	{
		acceptSockets();
		receiveData();
	}
}

// create socket
void Server::createSocket(void)
{
	// call socket function
	if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("socket() failed: " + std::string(strerror(errno)) + "\n");
	// set socket options
	int opt = 1;
	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt() failed: " + std::string(strerror(errno)) + "\n");
	// set socket non-blocking
	if (fcntl(serverfd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed: " + std::string(strerror(errno)) + "\n");
}

// bind socket
void Server::bindSocket(void)
{
	// initialize socket structure
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	// bind the host address using bind() call
	if (bind(serverfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		throw std::runtime_error("bind() failed: " + std::string(strerror(errno)) + "\n");
}

// listen socket
void Server::listenSocket(void)
{
	// start listening for the clients, here process will go in sleep mode and will wait for the incoming connection
	if (listen(serverfd, MAX_CLIENTS) < 0)
		throw std::runtime_error("listen() failed: " + std::string(strerror(errno)) + "\n");
	std::cout << "\n[+] Server is listening on port: " << BOLDGREEN << port << RESET << "\n" << std::endl;
}

// accept sockets
void Server::acceptSockets(void)
{
	// wait for an activity on one of the sockets, timeout is -1, so wait indefinitely
	if (poll(fds, nfds + 1, -1) < 0)
		throw std::runtime_error("poll() failed: " + std::string(strerror(errno)) + "\n");
	
	// if server socket is readable, then new client connection is request
	if (nfds < MAX_CLIENTS)
	{
		if (fds[0].revents & POLLIN)
		{
			t_fd newfd;
			Client newClient;
			struct sockaddr_in clientaddr;
			socklen_t addr_size = sizeof(clientaddr);
			// accept the connection
			newfd = accept(serverfd, (struct sockaddr *)&clientaddr, &addr_size);
			newClient.setAddr(clientaddr);
			newClient.setFd(newfd);
			clients[newfd] = newClient;
			fds[nfds + 1].fd = newfd;
			fds[nfds + 1].events = POLLIN;
			nfds++;
			std::cout << GREEN << "[+] New connection, socket fd is: " << RESET << BOLDYELLOW << newfd << RESET << "\n" << std::endl;
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
		for (t_clients_it clientPair = clients.begin(); clientPair != clients.end(); clientPair++)
		{
			t_fd clientFD = clientPair->first;
			if (fds[i + 1].revents & POLLIN && fds[i + 1].fd == clientFD)
			{
				int rcv = recv(clientFD, buffer, BUFFER_SIZE, 0);
				if (rcv > 0)
				{
					buffer[rcv] = '\0';
					// parse data
					printf("%s\n", buffer);
					parseData(i, clientFD, buffer);
				}
				// Client disconnected
				else if (rcv == 0)
				{
				// 	fds[i + 1].fd = 0;
				// 	close(clientFD);
				// 	// clients[clientFD] = Client();
				// 	clients[clientFD].setIsGetPassword(false);
				// 	std::cout << RED << "[-] Client disconnected, socket fd is: " << RESET << YELLOW << clientFD << RESET << "\n" << std::endl;

					closeConnection(i , clientFD);
				}
				// Handle recv() error
				else if (rcv == -1)
				{
					close(clientFD);
					fds[i + 1].fd = 0;
					throw std::runtime_error("recv() failed: " + std::string(strerror(errno)) + "\n");
				}
			}
		}
	}
}

// parse irc server commands
void Server::parseData(int i, t_fd fd, std::string data)
{
	std::string command = "";
	std::vector<std::string> params;
	static std::string oldData = "";
	
	// add data to oldData
	oldData += data;
	
	if (data.find('\n') != std::string::npos)
	{
		// remove \r\n and spaces from oldData
		oldData.erase(std::remove(oldData.begin(), oldData.end(), '\r'), oldData.end());
		oldData.erase(std::remove(oldData.begin(), oldData.end(), '\n'), oldData.end());
		oldData.erase(0, oldData.find_first_not_of(' '));

		if (oldData.find(':') != std::string::npos)
		{
			size_t pos = oldData.find(':');
			std::string str = oldData.substr(0, pos);
			str = ft::ft_replace(str, "  ", " ");
			oldData = str + oldData.substr(pos);
		}
		else 
		{
			oldData = ft::ft_replace(oldData, "  ", " ");
			// remove spaces from the end of oldData
			oldData.erase(oldData.find_last_not_of(' ') + 1);
		}

		params = ft::ft_split(oldData, " ");
		command = params[0];
		oldData = "";
	}
	
	// check commands and call functions
	// the PASS, NICK and USER commands uses to perform registration procedures for a new user
	// the PASS command must be the first command that a client sends to a server after it has connected to the server
	if (command == "PASS" || command == "pass")
		passCommand(fd, params);
	else if (command == "NICK" || command == "nick")
		nickCommand(i, fd, params);
	else if (command == "USER" || command == "user")
		userCommand(i, fd, params);
	else if (command == "QUIT" || command == "quit")
		quitCommand(i, fd, params);
	else if (command == "BIMO" || command == "bimo")
		bimoCommand(fd, params);
	else if (command == "JOIN" || command == "join")
		joinCommand(fd, params);
	else if (command == "MODE" || command == "mode")
		modeCommand(fd, params);
	else if (command == "PONG" || command == "pong")
		return ;
	else if (clients[fd].isRegistered())
	{
		// send irc server error message if the command is unknown
		sendData(fd, ERR_UNKNOWNCOMMAND(clients[fd].getNickname(), command));
	}

	// reset command and param
	command = "";
	params.clear();
	params.shrink_to_fit();
}

// send data
void Server::sendData(t_fd fd, std::string data)
{
	if (send(fd, data.c_str(), data.length(), 0) < 0)
		throw std::runtime_error("send() failed: " + std::string(strerror(errno)) + "\n");
}

// close Connection
void Server::closeConnection(int i, t_fd fd)
{
	fds[i + 1].fd = 0;
	close(fd);
	clients[fd] = Client();
	std::cout << RED << "[-] Client disconnected, socket fd is: " << RESET << YELLOW << fd << RESET << "\n" << std::endl;
}