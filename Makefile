# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zarran <zarran@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 17:38:31 by zarran            #+#    #+#              #
#    Updated: 2023/10/24 15:41:28 by zarran           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INCS		=	./incs/Server.hpp	\
				./incs/Colors.hpp	\
				./incs/Client.hpp	\
				./incs/ircserv.hpp	\
				./incs/Channel.hpp	
				
				# ./incs/Command.hpp	\
				# ./incs/Message.hpp	\

SRCS		=	./irc_main.cpp	\
				./srcs/Server.cpp	\
				./srcs/Client.cpp	\
				./srcs/Channel.cpp	
				
				# ./srcs/Command.cpp	\
				# ./srcs/Message.cpp	\


OBJS		=	$(SRCS:.cpp=.o)

NAME		=	ircserv
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
CXX			= 	c++
RM 		    = 	rm -rf

HEADER		=	./incs

all			: $(NAME)

$(NAME)		:	$(OBJS)
				$(CXX) $(CXXFLAGS) $(OBJS)  -o $(NAME)

%.o			:	%.cpp $(INCS)
				$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

clean		:
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re