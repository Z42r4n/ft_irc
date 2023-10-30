/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:33:35 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/10/30 16:08:44 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::passCommand(int i, t_fd fd, std::string param)
{ 
	if (password == param)
	{
		clients[i][fd].setIsRegistered(true);
		std::cout << "client registered" << std::endl;
	}
	else
	{
		this->sendData(fd, "ERROR :Invalid password\n");
	}
}