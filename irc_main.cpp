/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zarran <zarran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:45:14 by zarran            #+#    #+#             */
/*   Updated: 2023/10/20 16:14:00 by zarran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

namespace ft {
    void usage(void)
    { 
        std::cerr << BOLDRED << "Error:" << RESET << " invalid arguments\n" << std::endl;
        std::cout << "Usage: " << GREEN << "./ircserv" << RESET << " [port] [password]\n" << std::endl;
        exit (EXIT_FAILURE);
    }
}

int main(int ac, char **av)
{
    // irc server valid args
    if (ac != 3)
        ft::usage();
    try {
        Server irc_obj(std::atoi(av[1]), av[2]);
        
        std::cout << "password is: " << av[2] << std::endl;
        std::cout << "Everything is ok" << std::endl;
        irc_obj.run();
    } catch (std::exception &e) {
        std::cerr << BOLDRED << "\nError:" << RESET << " " << e.what() << std::endl;
        exit (EXIT_FAILURE);
    }
    
    exit (EXIT_SUCCESS);
}

