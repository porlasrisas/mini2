# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/07 15:57:01 by guigonza          #+#    #+#             #
#    Updated: 2025/09/02 20:55:31 by Guille           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
CC          = cc 
CFLAGS      = -Wall -Wextra -Werror
LDFLAGS 	= -lreadline

SRC_DIR     = src
OBJ_DIR     = obj
LIBFT_DIR   = libft
INCLUDES    = -I. -Iincludes -I$(SRC_DIR) -I$(LIBFT_DIR)

# Archivos fuente listados explícitamente

SRC = \
	builtins_advanced.c \
	builtins_basic.c \
	builtins_cd.c \
	builtins_dispatch.c \
	builtins_export.c \
	env.c \
	env_utils.c \
	env_utils_remove.c \
	error_utils.c \
	executor.c \
	executor_direct.c \
	executor_main.c \
	executor_path.c \
	export_utils.c \
	free_cmds.c \
	heredoc.c \
	heredoc_read.c \
	heredoc_utils.c \
	io_utils.c \
	main.c \
	parser_build.c \
	parser.c \
	parser_core.c \
	parser_expand.c \
	parser_loop_helpers.c \
	parser_pipe_checks.c \
	parser_redir.c \
	parser_redir_utils_io.c \
	parser_redir_utils_parse.c \
	parser_tokens.c \
	parser_utils.c \
	read_input.c \
	shell_loop.c \
	shell_loop_exec.c \
	signals.c \
	terminal.c



# Archivos objeto correspondientes
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

GREEN = \033[0;32m
YELLOW = \033[1;33m
RESET = \033[0m
RED = \033[0;31m

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(INCLUDES) -L$(LIBFT_DIR) -lft -lreadline
	@echo "$(GREEN)Compilación completada: $(NAME)$(RESET)"
	@echo "$(GREEN)---------------------------------------------------------------------------------------------------"
	@echo "$(YELLOW) ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓███████▓▒░░▒▓████████▓▒░░▒▓██████▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░    ░▒▓██▓▒░░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓▓█▓▒░░▒▓██▓▒░    ░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW)░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(YELLOW) ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░"
	@echo "$(GREEN)---------------------------------------------------------------------------------------------------"
	@echo "$(GREEN)\n ¡Enhorabuena champion, todo compilado!\n"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_DIR)/libft.a:
	@make -s -C $(LIBFT_DIR) bonus

clean:
	@make clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Objetos de minishell limpiados$(RESET)"

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)
	@echo "$(RED)Proyecto minishell completamente limpio$(RESET)"

re: fclean all

.PHONY: all clean fclean re
.SILENT:
