/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:01:48 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/19 17:43:58 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <termios.h>
# include <sys/wait.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include "./42-libft/libft.h"

# define BUFFER_SIZE 4096
# define GREEN "\001\033[32m\002"
# define RESET "\001\033[0m\002"

extern volatile sig_atomic_t	g_signal_received;

typedef struct s_token
{
	char			*cmd;
	int				type;
	char			**args_file;
	char			**args;
	struct s_token	*next;
	struct s_token	*prev;
	int				quoted;
	int				quoted_type;
	int				is_empty_quotes;
	int				is_invalid;
	int				is_literal;
	char			*infile;
	char			*outfile;
}	t_token;

typedef struct s_env
{
	char			*var;
	char			*content;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_arg
{
	int		i;
	char	*temp;
	char	*beg;
	char	*content;
}	t_arg;

typedef struct s_pipe_ctx
{
	int	**pipe_fds;
	int	cmd_index;
	int	count;
}	t_pipe_ctx;

typedef struct s_lexer
{
	t_token	*head;
	t_token	*tail;
	int		pos[2];
	int		in_quotes;
	char	quote_type;
	char	*input;
}	t_lexer;

typedef struct s_parser
{
	t_token	*curr;
	t_token	*prev;
	t_token	*last_cmd;
	int		cmd_seen;
}	t_parser;

typedef struct s_mini
{
	char		*input;
	char		*prompt;
	t_parser	*state;
	t_env		*export;
	t_token		*token;
	int			exit_status;
	int			exit_status_count;
	int			count;
	int			in_quotes;
	char		*current_cmd;
	char		*output;
	int			none;
	int			pipe;
	int			redirect;
	int			expr;
	int			expr_seen;
	int			heredoc;
	int			echo;
}	t_mini;

typedef enum e_cmd_type
{
	CMD_NONE = 0,
	CMD_PIPE = 10,
	CMD_EXEC = 11,
	CMD_BUILDIN = 12,
	CMD_ARG = 13,
	CMD_ARG_FILE = 14,
	CMD_REDIRECT = 15,
	CMD_REDIRECT_IN = 16,
	CMD_REDIRECT_OUT = 17,
	CMD_HEREDOC = 18,
	CMD_SINGLE_QUOTE = 19,
	CMD_DOUBLE_QUOTE = 20,
	CMD_EXIT_STATUS = 21,
	CMD_EXPR = 23,
	CMD_PLUS = 24
}	t_cmd_type;

//Core initialization
t_mini	init(char **envp);
t_env	*ft_last(t_env *head);
char	*get_input(t_mini *ms, char *prompt);
void	ft_update_ms(t_mini *ms);

//Lexer & Parsing
t_token	*lexer(char *input);
void	ft_init_lexer_state(t_lexer *state, char *input);
int		process_char(t_lexer *state);
int		create_operator_token(t_lexer *state, int *i);
t_token	*create_new_token(char *cmd);
t_token	*add_previous_token(t_lexer *state);
int		parser(t_parser *state, t_mini *ms);
void	process_token(t_parser *state, t_mini *ms);
void	process_token_part2(t_parser *state, t_mini *ms);
void	ft_handle_spec(t_parser *state, t_mini *ms);
void	ft_handle_norm(t_parser *state, t_mini *ms);
void	process_quotes(t_token *token);
void	build_args_from_tokens(t_token *cmd);
void	set_command_type(t_token *current);
void	free_token_list(t_token *head);

//Token Processing
void	process_expr_command(t_token *current, t_mini *ms);
int		ft_check_args_validity(t_token *current, t_mini *ms);
int		ft_determine_expression_type(t_token *current, t_mini *ms);
int		ft_just_one_arg(t_token *current, t_mini *ms);
int		ft_is_valid_operator_token(t_token *token);
void	process_exit_status(t_token *current, t_mini *ms);
int		evaluate_term(const char *term, t_mini *ms);
int		ft_handle_exit_status(t_mini *ms);
int		ft_is_valid_operator(char *symbol);
int		handle_operator(char *op, t_token **current, int result, t_mini *ms);
void	ft_handle_unknown_operator(t_token *current);
void	mark_tokens_as_processed(t_token *start);

//Argument Handling
void	add_to_args_file(t_token *token, char *arg);
void	add_to_args(t_token *token, char *arg);

//Token Type Handling
void	handle_command_token(t_parser *state, t_mini *ms);
void	handle_arg_token(t_parser *state, t_mini *ms);
void	handle_exit_status_argument(t_token *current, t_token *last_cmd,
			t_mini *ms);
//Setup Instructions
void	setup_expr_command(t_token *current,
			int *command_seen, t_token **last_cmd);
void	setup_command_after_exit_status(t_parser *state);
void	ft_handle_zero(t_mini *ms);

//Execution
void	exec(t_mini *ms);
void	ft_exec_token_list(t_mini *ms);
int		ft_handle_token(t_token *current, t_token *prev, t_mini *ms);
int		execute_command(t_token *cmd, t_mini *ms);
char	*find_command_path(const char *cmd, t_mini *ms);
int		loop(t_token *cmd, int *stdout_backup);
int		handle_redirect_out(t_token *file_token, int stdout_backup);
int		ft_execute_child(t_token *cmd, t_mini *ms);
void	execute_child_process(t_token *current, t_mini *ms);
int		ft_execute_parent(t_mini *ms, pid_t pid);
int		is_builtin(const char *cmd);

//Builtins
int		exec_builtin(t_token *token, t_mini *ms);
void	exec_pwd(t_token *token);
void	exec_unset(t_token *token, t_mini *mini);
void	exec_env(t_token *token, t_mini *mini);
void	exec_exit(t_token *token, t_mini *ms);
//Echo
void	exec_echo(t_token *token, t_mini *mini);
int		has_redirection(t_token *token);
char	*ft_get_redirect_pos(char *input_start);
void	ft_handle_redirect_case(char *input_start, char *redirect_pos);
char	*ft_custom_strstr(const char *haystack, const char *needle);
char	*ft_custom_substr(char const *s, unsigned int start, size_t len);
//Export
void	exec_export(t_token *token, t_mini *mini);
void	ft_init_arg_parts(t_arg *parts, char *input, t_mini *mini);
//CD
void	exec_cd(t_token *token, t_mini *mini);
int		list_size(t_token *token);
char	*get_new_cwd(char *buffer);
void	exec_cd_point(t_token *token, t_mini *mini);
void	cd_dollar(t_token *next, t_mini *mini);
void	update_var(char *oldpwd, char *pwd, t_mini *mini);
int		check_fullpath(t_token *next, t_mini *mini);

//Redirection & Pipes
int		exec_pipe(t_mini *ms);
int		**init_pipes(int pipe_count);
void	handle_child_process(t_token *current, t_mini *ms, t_pipe_ctx *ctx);
void	close_and_free_pipes(t_pipe_ctx *ctx);
int		exec_redirect(t_token *token, t_mini *ms);
int		exec_heredoc(t_token *token, t_mini *ms);
void	setup_pipe_token(t_token *current, t_mini *ms);
void	setup_redirect_out_token(t_token *current, t_mini *ms);
void	setup_redirect_in_token(t_token *current, t_mini *ms);

//Environmental variables
t_env	*find_node(char *var, t_env *head);
char	*expand_var(char *var, t_env *ev);
void	append_node(char *var, char *content, t_env *head);
void	order_var(t_mini *mini);
void	print_export(t_env *head);

//Signal Handling
void	disable_ctrl_backslash(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signals(void);
void	setup_exec_signals(void);
void	reset_signals(void);

//Errors Handling
int		handle_fork_error(int stdout_backup);

//Memory Management
void	free_tokens(t_token *token);
void	ft_free_minishell(t_mini *ms, int code);
void	free_2strings(char *var, char *arg);
char	**free_mat(char **mat);
void	free_pwd(char *oldpwd, char *pwd);
void	free_env_list(t_env *head);

//Utils
int		ft_strcmp(char *s1, char *s2);
int		is_redirect_out(char *cmd);
int		is_redirect_in(char *cmd, t_mini *ms);
int		is_exec_command(char *cmd);
int		is_builtin_command(char *cmd);
int		is_math_operator(t_token *current);
int		is_in_expr_context(t_token *prev);
void	ft_handle_flag(t_token **next, int *nl_flag);
int		ft_is_valid_n_flag(char *s, int *len);
int		ft_skip_n_flags(char *input, int *nl_flag);
void	set_child_pipes(int **pipe_fds, int cmd_index, int pipe_count);
int		exec_pipe_with_redirects(t_mini *ms);
t_token	*ft_unlink_tokens(t_token *token, t_token *next);
int		ft_open_redirect_file(t_token *token, t_token *next);
int		ft_redir_exec_setup(int fd, int is_input);
char	**env_to_array(t_env *env);
void	free_env_array(char **env_array);
void	free_args_array(char **args);
int		exec_heredoc_with_pipes(t_mini *ms);
int		exec_heredoc_with_pipes(t_mini *ms);
int		process_special_token2(t_token *token, t_mini *ms,
			int *has_output_redirect);
void	setup_child_pipes(int **pipe_fds, int cmd_index,
			int pipe_count, int has_output_redirect);
int		handle_redirect(t_token *token, t_mini *ms);
int		setup_heredoc(t_token *token, t_mini *ms);
t_token	*find_command(t_token *current);
void	execute_child(t_token *start_token, t_mini *ms, t_pipe_ctx *ctx);
t_token	*next_command(t_token *current);
int		process_command(t_token **current, t_mini *ms, t_pipe_ctx *ctx);
int		execute_pipeline(t_mini *ms, t_pipe_ctx *ctx);
int		**init_pipes2(int pipe_count);
void	close_pipes(t_pipe_ctx *ctx);
void	setup_child_pipes(int **pipe_fds, int cmd_index,
			int pipe_count, int has_output_redirect);
void	execute_child(t_token *start_token, t_mini *ms, t_pipe_ctx *ctx);
int		process_command(t_token **current, t_mini *ms, t_pipe_ctx *ctx);
int		execute_pipeline(t_mini *ms, t_pipe_ctx *ctx);
int		update_exit_status(t_mini *ms, int status);
void	close_all_pipe_fds(int **pipe_fds, int pipe_count);
void	dup_pipe_ends(int **pipe_fds, int cmd_index,
			int pipe_count, int has_output_redirect);
void	execute_child(t_token *start_token, t_mini *ms, t_pipe_ctx *ctx);
void	execute_child_part2(t_token *cmd_token, t_mini *ms,
			t_pipe_ctx *ctx, int has_output_redirect);
t_token	*find_command(t_token *current);
int		process_special_token2(t_token *token, t_mini *ms,
			int *has_output_redirect);
int		ft_handle_heredoc_input(const t_token *next, int fd[2]);
int		ft_execute_builtin(t_token *cmd_token, t_mini *ms, int fd);
int		ft_execute_external(t_token *cmd_token, t_mini *ms, int fd);
int		ft_heredoc_default(int fd);
void	handle_execve_with_args(t_token *current, t_mini *ms, char **envp);
int		ft_execute_special_in_child(t_token *current, t_mini *ms);
void	set_child_pipes(int **pipe_fds, int cmd_index, int pipe_count);
void	handle_execve_with_cmd(t_token *current, t_mini *ms, char **envp);
void	handle_execve_error(char *path, char **envp);
int		ft_redirect_execution(t_token *cmd, t_mini *ms,
			int fd, int is_input);
void	ft_relink_tokens(t_token *token, t_token *next, t_token *after);

void	close_pipe_ends(t_pipe_ctx *ctx);
void	close_all_pipes_in_child(t_pipe_ctx *ctx);
void	prepare_child_io(t_pipe_ctx *ctx);
int		setup_redirects(t_token *token, t_token **next_cmd);
int		setup_redirect_single(t_token *token, t_token *next,
			t_token **after);
int		process_redirect_file(t_token *token, t_token *next);
void	close_and_free_pipes1(t_pipe_ctx *ctx);
int		**init_pipes1(int count);
void	handle_child_process1(t_token *cmd, t_mini *ms, t_pipe_ctx *ctx);
void	display_command_not_found(t_token *token, t_mini *ms);
int		is_valid_for_processing(t_mini *ms);
void	ft_decide_on_exit_status(t_parser *state, t_mini *ms);
void	free_token_content(t_token *token);
void	execute_cmd(t_token *cmd, t_mini *ms);
void	ft_exec_token_list(t_mini *ms);
void	process_special_token(t_token *special_token, t_mini *ms);
int		ft_handle_token(t_token *current, t_token *prev, t_mini *ms);
t_token	*find_special_token(t_token *current);
int		handle_input_line(char *line, t_mini *ms);
void	restore_ctrl_c_only(void);

#endif
