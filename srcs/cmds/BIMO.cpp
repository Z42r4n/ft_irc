/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BIMO.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 09:16:47 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/11/04 13:17:06 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

void Server::bimoCommand(int i, t_fd fd, t_params params)
{	
	std::string api = "";

	if ((params[1] == "figlet" || params[1] == "FIGLET") && (params.size() > 3))
	{
		std::string yourText = params[3];
		if (params.size() > 3)
			yourText = ft::ft_getStrFromIndex(3, params);
		std::string font =  params[2];
		api = "curl -s \'https://figlet-api.onrender.com/ascii?text=" + yourText + "&font=" + font + "\' | jq -r \'.ascii\' | sed -e \'/^[[:space:]]*$/d\'";
	}
	else if (params[1] == "fonts" || params[1] == "FONTS")
		api = "curl -s \'https://figlet-api.onrender.com/fonts\' | jq -r \'.[]\' | tr \' \' \'\n\'";
	else if (params[1] == "weather" || params[1] == "WEATHER")
		api = "curl -s \'wttr.in/{Rabat,London,Berlin,Madrid,Paris,Moscow,Oslo,Beijing,Ottawa}?format=3\'";
	else
	{
		sendData(fd, ERR_NEEDMOREPARAMS(clients[i][fd].getNickname(), params[0]));
		sendData(fd, RPL_MOTD(clients[i][fd].getNickname(), "Usage: /BIMO [fonts])"));
		sendData(fd, RPL_MOTD(clients[i][fd].getNickname(), "Usage: /BIMO [figlet] [font] [text])"));
		sendData(fd, RPL_MOTD(clients[i][fd].getNickname(), "Usage: /BIMO [weather])"));
		return ;
	}

	// store the output of the figlet fonts command in a file
	system((api + " > api.txt").c_str());

	// read the file and send the output to the client.
	std::ifstream file("api.txt");
	std::string line;

	// send the motd message to the client
	sendData(fd, RPL_MOTD(clients[i][fd].getNickname(), "BIMO BOT RESPONSE: ------------------"));

	// read the file line by line ans send the output to the client
	while (std::getline(file, line))
	{
		sendData(fd, RPL_MOTD(clients[i][fd].getNickname(), line));
	}
	
	// close the file
	file.close();
	
	// remove the file
	system("rm api.txt");
}