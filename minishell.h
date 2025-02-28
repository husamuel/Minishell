#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_token
{
	char			*cmd;
	int				type;
	char			**args_file;
	struct s_token	*next;
	struct s_token	*prev;
	int quoted;
	int quoted_type;
}	t_token;

// RUI
typedef struct s_env
{
	char			*var;
	char			*content;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;
///

typedef struct s_mini
{
	char	*input;
	char	*prompt;
	t_env	*envp;
	t_env	*export;
	t_token	*token;
	int exit_status;
	int in_quotes;
}	t_mini;

typedef enum e_cmd_type {
    CMD_NONE = 0,
    CMD_PIPE = 10,
    CMD_EXEC = 11,
    CMD_BUILDIN = 12,
    CMD_ARG = 13,
    CMD_ARG_FILE = 14,
    CMD_REDIRECT = 15,
    CMD_HEREDOC = 16,
    CMD_SINGLE_QUOTE = 17,
    CMD_DOUBLE_QUOTE = 18,
    CMD_EXIT_STATUS = 19,
    CMD_SUBSHELL = 20
} t_cmd_type;


# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <sys/wait.h>
#include <string.h>
# include <unistd.h>
# include "./42-libft/libft.h"

#define BUFFER_SIZE 4096

// RUI functions -- CD functions
char	*get_new_cwd(char *buffer);
void	free_pwd(char *oldpwd, char *pwd);

int		ft_strcmp(char *s1, char *s2);

void	update_var(char *oldpwd, char *pwd, t_mini *mini);
char	*expand_var(char *var, t_env *ev);

t_env	*ft_last(t_env *head);
/////

t_mini init(char **envp);
char *get_input(t_mini *ms, char *prompt);
t_token *lexer(char *input);
void parser(t_mini *ms);
void add_to_args_file(t_token *token, char *arg);

void process_token(t_token *current, t_token *prev,
    t_token **last_cmd, int *command_seen, t_mini *ms);
void handle_syntax_error(t_mini *ms);
void    exec(t_mini *ms);
void    exec_pipe(t_token *token);
void    exec_builtin(t_token *token, t_mini *ms);
void    exec_redirect(t_token *token);
void    exec_heredoc(t_token *token);

void     exec_echo(t_token *token, t_mini *mini);
void     exec_cd(t_token *token, t_mini *mini);
void     exec_pwd(t_token *token);
void     exec_export(t_token *token, t_mini *mini);
void     exec_unset(t_token *token, t_mini *mini);
void     exec_env(t_token *token, t_mini *mini);
void     exec_exit(t_token *token);

int is_builtin_command(const char *cmd);
int is_exec_command(const char *cmd);
void set_command_type(t_token *current);
extern volatile sig_atomic_t g_signal_received;

void    sigint_handler(int sig);
void    sigquit_handler(int sig);
void    setup_signals(void);
void	free_tokens(t_token *token);
void execute_command(t_mini *ms, t_token *cmd);

#endif