/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:22:19 by ymoutaou          #+#    #+#             */
/*   Updated: 2023/10/31 17:35:45 by ymoutaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ircserv.hpp>

namespace ft 
{
	std::vector<std::string> ft_split(std::string str, std::string delim)
	{
		std::vector<std::string> result;
		size_t pos = 0;
		std::string token;
		while ((pos = str.find(delim)) != std::string::npos)
		{
			token = str.substr(0, pos);
			result.push_back(token);
			str.erase(0, pos + delim.length());
		}
		result.push_back(str);
		return result;
	}

	std::string ft_getStr(t_params params)
	{
		std::string str = "";

		for (size_t i = 0; i < params.size(); i++)
		{
			if (params[i].find(":") != std::string::npos)
			{
				// join the rest of the string and the rest of params ignoring :
				for (size_t j = i; j < params.size(); j++)
				{
					str += params[j];
					if (j + 1 < params.size())
						str += " ";
				}
			}
		}
		return str.substr(1, str.length());
	}
}