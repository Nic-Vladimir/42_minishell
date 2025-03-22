# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/03 19:12:35 by vnicoles          #+#    #+#              #
#    Updated: 2025/03/18 22:47:14 by vnicoles         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Stfu make ---
MAKEFLAGS += --no-print-directory

# --- Colors ---
PURPLE	= \033[38;5;141m
GREEN	= \033[38;5;46m
RED		= \033[0;31m
GREY	= \033[38;5;240m
RESET	= \033[0m
BOLD	= \033[1m
CLEAR	= \r\033[Ks

# --- Vars ---
NAME		= minishell
CC			= gcc -g
CFLAGS		= -Wall -Wextra -Werror -I $(INC_DIR)

# --- Paths ---
INC_DIR		= inc/
SRC_DIR		= src/
LIB_DIR		= lib/
OBJ_DIR		= obj/
LIBFT_DIR	= $(LIB_DIR)/libft
LIBFT		= $(LIB_DIR)/libft/libft.a
SRC			= main.c \
				ast.c \
				arena.c \
				tokenizer.c \
				env.c \
				hashmap.c \
				exec_engine.c \

OBJ			= $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
HEADERS		= $(INC_DIR)minishell.h

# --- Progress bar ---
TOTAL_FILES	= $(words $(SRC))
CURR_FILE	= 0
PERCENT		= $(shell echo "$(CURR_FILE) * 100 / $(TOTAL_FILES)" | bc)

define update_progress
	@$(eval CURR_FILE = $(shell echo "$(CURR_FILE) + 1" | bc))
	@$(eval PERCENT = $(shell echo "$(CURR_FILE) * 100 / $(TOTAL_FILES)" | bc))
	@printf "\r\033[K"
	@printf "$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)MINISHELL$(RESET)]: \tCompiling... ["
	@printf "%*s" $(shell echo "$(PERCENT) * 20 / 100" | bc) "" | tr ' ' '='
	@printf "%*s" $(shell echo "20 - ($(PERCENT) * 20 / 100)" | bc) "" | tr ' ' '.'
	@printf "] %3d%% %s" $(PERCENT)
	@printf " %s" $(notdir $<)
endef

# --- Rules ---
all:  $(OBJ_DIR) $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	@printf "\r\033[K$(RESET)$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)MINISHELL$(RESET)]: \tCompiled  ["
	@printf "%*s" 20 "" | tr ' ' '='
	@printf "] "
	@printf "100%% "
	@printf "$(GREEN)Complete!$(RESET)\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call update_progress)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	
	@printf "$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)MINISHELL$(RESET)]: \tCleaned\n"

fclean: clean
	@rm $(NAME)
	@rm $(LIBFT)
	@printf "$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)MINISHELL$(RESET)]: \t$(GREEN)Full clean$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
