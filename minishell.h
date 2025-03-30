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
	int is_invalid;
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
	t_env	*export;
	t_token	*token;
	int exit_status;
	int in_quotes;
	char *current_cmd;
	char *output;
	int pipe;
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
    CMD_SUBSHELL = 20,
	CMD_EXPR = 21,
	CMD_PLUS = 22
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
void	update_var(char *oldpwd, char *pwd, t_mini *mini);

// UTILS
void	free_2strings(char *var, char *arg);
int		ft_strcmp(char *s1, char *s2);
int		list_size(t_token *token);
t_env	*ft_last(t_env *head);
t_env	*find_node(char *var, t_env *head);

// EXPAND
char	*expand_var(char *var, t_env *ev);

// EXPORT
void	append_node(char *var, char *content, t_env *head);
void	order_var(t_mini *mini);
void	print_export(t_env *head);

// CD FUNCTIONS
void	exec_cd_point(t_token *token, t_mini *mini);
int		check_fullpath(t_token *next, t_mini *mini);
void	cd_dollar(t_token *next, t_mini *mini);
//

t_mini init(char **envp);
char *get_input(t_mini *ms, char *prompt);
t_token *lexer(char *input);
void parser(t_mini *ms);
void add_to_args_file(t_token *token, char *arg);

void process_token(t_token *current, t_token *prev,
    t_token **last_cmd, int *command_seen, t_mini *ms);
void    exec(t_mini *ms);
int exec_pipe(t_mini *ms);
int    exec_builtin(t_token *token, t_mini *ms);
int    exec_redirect(t_token *token);
int    exec_heredoc(t_token *token);

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
int execute_command(t_token *cmd);
void handle_exit_status(t_mini *ms, int status);
void process_expr_command(t_token *current, t_mini *ms);
#endif