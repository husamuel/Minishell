CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./42-libft
LIBS = -lreadline -L./42-libft -lft
SRCS = main.c init.c lexer.c input.c parser.c executable.c expr.c \
       ./commands/exec_heredoc.c ./commands/exec_pipe.c \
       ./commands/exec_redirect.c ./commands/exec_builtin.c \
       ./commands/builtin/exec_cd.c ./commands/builtin/exec_cd_2.c \
	   ./commands/builtin/exec_cd_3.c \
	   ./commands/builtin/exec_echo.c \
       ./commands/builtin/exec_exit.c ./commands/builtin/exec_env.c \
       ./commands/builtin/exec_export.c ./commands/builtin/export_utils.c \
	   ./commands/builtin/export_3.c ./commands/builtin/export_4.c \
	   ./commands/builtin/exec_pwd.c \
       ./commands/builtin/exec_unset.c handle_token.c free.c exit_status.c

OBJS = $(SRCS:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	@make -C ./42-libft
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make clean -C ./42-libft
	rm -f $(OBJS)

fclean: clean
	@make fclean -C ./42-libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
