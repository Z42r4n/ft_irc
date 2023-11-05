# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymoutaou <ymoutaou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 17:38:31 by zarran            #+#    #+#              #
#    Updated: 2023/11/05 13:21:47 by ymoutaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# define colors
GREEN		=	\033[0;32m
RED			=	\033[0;31m
YELLOW		=	\033[0;33m
BLUE		=	\033[0;34m
MAGENTA		=	\033[0;35m
CYAN		=	\033[0;36m
RESET		=	\033[0m

# define text style
BOLD		=	\033[1m
DIM			=	\033[2m
UNDERLINED	=	\033[4m
BLINK		=	\033[5m

# define headers
INCS		=	./incs/Server.hpp		\
				./incs/Colors.hpp		\
				./incs/Client.hpp		\
				./incs/ircserv.hpp		\
				./incs/Channel.hpp	
				
				# ./incs/Command.hpp	\
				# ./incs/Message.hpp	\

# define sources
SRCS		=	./irc_main.cpp			\
				./srcs/Server.cpp		\
				./srcs/Client.cpp		\
				./srcs/Channel.cpp		\
				./srcs/cmds/PASS.cpp	\
				./srcs/cmds/NICK.cpp	\
				./srcs/cmds/USER.cpp	\
				./srcs/cmds/QUIT.cpp	\
				./srcs/cmds/BIMO.cpp	\
				./srcs/cmds/JOIN.cpp	\
				./srcs/cmds/MODE.cpp	\
				./srcs/utils/tools.cpp	
				
				# ./srcs/Command.cpp	\
				# ./srcs/Message.cpp	\


# define objects
OBJS		=	$(SRCS:.cpp=.o)

# define executable
NAME		=	ircserv

# define flags
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
CXX			= 	c++
RM 		    = 	rm -rf

# define includes
HEADER		=	./incs

all			: logo $(NAME)

logo		:
				@echo "$(CYAN)"
				@if [ -x /Users/ymoutaou/figlet/figlet ]; then \
					/Users/ymoutaou/figlet/figlet -f /Users/ymoutaou/figlet/fonts/Bloody "IRC SERVER" | lolcat -a -d 5; \
					echo "By [ymoutaou] and [houadou]" | lolcat -a -d 5; \
				else \
					echo "[ IRC SERVER ]"; \
					echo "By [ymoutaou] and [houadou]"; \
				fi
				@echo "$(RESET)"

$(NAME)		:	$(OBJS)
				@if which lolcat > /dev/null; then \
					echo "Linking   . . . [$(OBJS)] -> [$@]" | lolcat -a -d 5; \
				else \
					echo "$(YELLOW)Linking   . . .$(RESET) [$(MAGENTA)$(OBJS)$(RESET)] -> [$(CYAN)$(BOLD)$(NAME)$(RESET)]"; \
				fi
				@$(CXX) $(CXXFLAGS) $(OBJS)  -o $(NAME)

%.o			:	%.cpp $(INCS)
				@if which lolcat > /dev/null; then \
					echo "Compiling . . . [$<] -> [$@]" | lolcat -a -d 5; \
				else \
					echo "$(GREEN)Compiling . . .$(RESET) [$(MAGENTA)$<$(RESET)] -> [$(YELLOW)$@$(RESET)]"; \
				fi
				@$(CXX) $(CXXFLAGS) -I $(HEADER) -c $< -o $@

clean		:
				@if which lolcat > /dev/null; then \
					echo "Cleaning . . . [$(OBJS)]" | lolcat -a -d 5; \
				else \
					echo "$(RED)Cleaning . . .$(RESET) [$(YELLOW)$(OBJS)$(RESET)]"; \
				fi
				@$(RM) $(OBJS)

fclean		:	logo clean
				@if which lolcat > /dev/null; then \
					echo "Removing . . . [$(NAME)]" | lolcat -a -d 5; \
				else \
					echo "$(RED)Removing . . .$(RESET) [$(BLINK)$(NAME)$(RESET)]"; \
				fi
				@$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re