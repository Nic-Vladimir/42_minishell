MAKEFLAGS += --no-print-directory

# Colors constants
PURPLE			= \033[38;5;141m
GREEN			= \033[38;5;46m
RED				= \033[0;31m
GREY			= \033[38;5;240m
RESET			= \033[0m
BOLD			= \033[1m
CLEAR			= \r\033[K

SRCS = ft_atoi.c \
	ft_bzero.c \
	ft_calloc.c \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_itoa.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memset.c \
	ft_putchar_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_putstr_fd.c \
	ft_split.c \
	ft_strchr.c \
	ft_strdup.c \
	ft_striteri.c \
	ft_strjoin.c \
	ft_strjoin_free.c \
	ft_strlcat.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strmapi.c \
	ft_strncmp.c \
	ft_strnstr.c \
	ft_strrchr.c \
	ft_strtrim.c \
	ft_substr.c \
	ft_tolower.c \
	ft_toupper.c
BSRCS = ft_lstadd_back.c \
		ft_lstadd_front.c \
		ft_lstclear.c \
		ft_lstdelone.c \
		ft_lstiter.c \
		ft_lstlast.c \
		ft_lstmap.c \
		ft_lstnew.c \
		ft_lstsize.c \
		get_next_line.c \
		ft_printf.c \
		ft_printf_utils_1.c \
		ft_printf_utils_2.c

SRC_DIR       = ./src/
INC_DIR       = ./inc/
OBJ_DIR       = ./obj/
OBJS	       = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))
BOBJS	       = $(addprefix $(OBJ_DIR), $(BSRCS:.c=.o))

CC			= gcc
RM			= rm -rf
CFLAGS		= -g -Wall -Wextra -Werror -I $(INC_DIR)
NAME		= libft.a

# Calculate total number of files and create variables for the progress bar
TOTAL_FILES	= $(words $(SRCS) $(BSRCS))
CURR_FILE	= 0
PERCENT		= $(shell echo "$(CURR_FILE) * 100 / $(TOTAL_FILES)" | bc)

# Define the progress bar function (40 chars width)
define update_progress
		@$(eval CURR_FILE = $(shell echo "$(CURR_FILE) + 1" | bc))
		@$(eval PERCENT = $(shell echo "$(CURR_FILE) * 100 / $(TOTAL_FILES)" | bc))
		@printf "\r\\033[K"
		@printf "$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)LIBFT$(RESET)]: \tCompiling... ["
		@printf "%*s" $(shell echo "$(PERCENT) * 20 / 100" | bc) "" | tr ' ' '='
		@printf "%*s" $(shell echo "20 - ($(PERCENT) * 20 / 100)" | bc) "" | tr ' ' '.'
		@printf "] %3d%% %s" $(PERCENT)
		@printf " %s" $(notdir $<)
endef

all:		$(OBJ_DIR) $(NAME)

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c
		@$(CC) $(CFLAGS) -c $< -o $@
		$(call update_progress)

$(NAME):	$(OBJS) $(BOBJS)
		@ar rcs $(NAME) $(OBJS) $(BOBJS)
		@printf "\r\\033[K$(CLEAR)$(RESET)$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)LIBFT$(RESET)]: \tCompiled  ["
		@printf "%*s" 20 "" | tr ' ' '='
		@printf "] "
		@printf "100%% "
		@printf "$(GREEN)Complete!$(RESET)"
		@printf "\n"


clean:
		@$(RM) $(OBJ_DIR)
		@printf "$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)LIBFT$(RESET)]: \tRemoved $(BOLD)OBJ$(RESET)\n"

fclean:	clean
		@$(RM) $(NAME)
		@printf "$(GREEN)»$(RESET) [$(PURPLE)$(BOLD)LIBFT$(RESET)]: \tRemoved $(BOLD)libft.a$(RESET)\n"

re:		fclean all

.PHONY:	all clean fclean re
