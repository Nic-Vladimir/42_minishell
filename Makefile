# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/03 19:12:35 by vnicoles          #+#    #+#              #
#    Updated: 2025/08/18 11:20:01 by mgavorni         ###   ########.fr        #
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
CC			= cc -g 
CFLAGS		= -Wall -Wextra -Werror -I $(INC_DIR)

# --- Paths ---
INC_DIR		= inc/
SRC_DIR		= src/
LIB_DIR		= lib/
OBJ_DIR		= obj/
LIBFT_DIR	= $(LIB_DIR)/libft
LIBFT		= $(LIB_DIR)/libft/libft.a
SRC			= main.c \
				ast/ast.c \
				ast/redirections.c \
				ast/parse_simple_command.c \
				ast/init_node.c \
				env/ft_getpid.c \
				env/shell_env.c \
				env/env_utils.c \
				env/hmap_insert.c\
				env/free_hmap.c \
				env/hashmap.c \
				exec_engine/exec_engine.c \
				exec_engine/exec_engine_utils.c \
				exec_engine/exec_engine_utils2.c \
				exec_engine/exec_pipeline_utils.c \
				exec_engine/exec_pipeline.c \
				exec_engine/exec_command.c \
				exec_engine/heredoc_utils2.c \
				exec_engine/heredoc_utils.c \
				exec_engine/pedo.c\
				exec_engine/pedo_utils.c \
				exec_engine/heredoc.c \
				exec_engine/redirections.c \
				exec_engine/expand_variable.c \
				exec_engine/expand_command.c \
				exec_engine/arg_utils.c \
				exec_engine/copy_args.c \
				utils/prompt.c \
				utils/wildcard.c \
				cmds/export.c \
				cmds/export_utils.c \
				cmds/export_quick_sort.c \
				cmds/env.c \
				cmds/pwd.c \
				cmds/echo.c \
				cmds/cd.c \
				cmds/exit.c \
				cmds/unset.c \
				signals/signals.c \
				signals/signals_handlers.c \
				tokenizer/tokenizer.c \
				tokenizer/tokenizer_filters.c \
				tokenizer/tokenizer_utils.c \
				tokenizer/validate_syntax.c \
				tokenizer/validate_syntax_utils.c \
				tokenizer/free_tok_data.c \
				comprehensive_cleanup.c \
				comprehensive_child.c \
				cleanup_copy.c \

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
	@mkdir -p obj/cmds/
	@mkdir -p obj/signals/
	@mkdir -p obj/tokenizer/
	@mkdir -p obj/exec_engine/
	@mkdir -p obj/env/
	@mkdir -p obj/ast/
	@mkdir -p obj/utils/

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
