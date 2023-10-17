/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:45:14 by zarran            #+#    #+#             */
/*   Updated: 2023/10/16 19:50:30 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <Colors.hpp>
#include <Server.hpp>

void usage(void)
{ 
    std::cerr << BOLDRED << "\nError:" << RESET << " invalid arguments\n" << std::endl;
    std::cout << "Usage: " << GREEN << "./ircserv" << RESET << " [port] [password]\n" << std::endl;
    exit (EXIT_FAILURE);
}

int main(int ac, char **av)
{
    // irc server valid args
    if (ac != 3 || !av[1] || !av[2])
    {
        // here must be a check for port is valid or not
        usage();
    }
    // create server object
    // Server irc_server(av[1], av[2]);
    
    exit (EXIT_SUCCESS);
}