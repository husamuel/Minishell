/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:01:48 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/03 18:17:34 by gtretiak         ###   ########.fr       */
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

extern int	g_exit_status;

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
	char			**pipes_cmd;
	int				*pipes_type;
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
	CMD_APPEND = 15,
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
int		ft_strcmp(char *s1, char *s2);
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
int		process_expr_command(t_token *current, t_mini *ms);
int		ft_check_args_validity(t_token *current, t_mini *ms);
int		ft_determine_expression_type(t_token *current, t_mini *ms);
int		handle_standalone(t_token *current);
int		ft_just_one_arg(t_token *current, t_mini *ms);
int		ft_is_valid_operator_token(t_token *token);
void	process_exit_status(t_token *current);
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
void	handle_exit_status_argument(t_token *current, t_token *last_cmd);
//Setup Instructions
void	setup_expr_command(t_token *current,
			int *command_seen, t_token **last_cmd);
void	ft_handle_zero(t_mini *ms);
void	setup_command_after_exit_status(t_parser *state);
//Execution
void	exec(t_mini *ms);
int		is_builtin(const char *cmd);

//Builtins
int		exec_builtin(t_token *token, t_mini *ms);
void	exec_pwd(t_token *token);
void	exec_unset(t_token *token, t_mini *mini);
void	exec_env(t_token *token, t_mini *mini);
void	exec_exit(t_token *token);
void	exec_cd(t_token *token, t_mini *mini);
//Echo
void	exec_echo(t_token *token, t_mini *mini);
int		has_redirection(t_token *token);
char	*ft_get_redirect_pos(char *input_start);
void	ft_handle_redirect_case(char *input_start, char *redirect_pos);
int		has_unclosed_quotes(const char *input);
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
int		is_core_dumped(int status);

//Errors Handling
int		handle_fork_error(int stdout_backup);

//Memory Management
void	free_tokens(t_token *token);
void	ft_free_minishell(t_mini *ms, int code);
void	free_2strings(char *var, char *arg);
char	**free_mat(char **mat);
void	free_pwd(char *oldpwd, char *pwd);
void	free_env_list(t_env *head);
void	ft_free_pipes_data(t_token *head);
void	ft_free_cmds(char **arr);

//Utils
int		ft_strcmp(char *s1, char *s2);
int		is_redirect_out(char *cmd);
int		is_exec_command(char *cmd);
int		is_builtin_command(char *cmd);
int		is_math_operator(t_token *current);
int		is_in_expr_context(t_token *prev);
void	ft_handle_flag(t_token **next, int *nl_flag);
int		ft_is_valid_n_flag(char *s, int *len);
int		ft_skip_n_flags(char *input, int *nl_flag);
char	**env_to_array(t_env *env);
char	*expand_and_free(char *str, t_mini *mini);
void	free_args_array(char **args);
void	display_command_not_found(t_token *token, t_mini *ms);
int		is_valid_for_processing(t_mini *ms);
void	ft_decide_on_exit_status(t_parser *state, t_mini *ms);
void	free_token_content(t_token *token);
int		handle_input_line(char *line);
void	restore_child_tty(void);
char	**get_env_array(t_env *env);
int		setup_heredoc(char *delimiter);
void	ft_free_split(char **split);
char	*ft_strjoin_free(char *s1, char *s2);
char	*get_env_value(t_env *env, char *key);
int		setup_redirections(t_token *token);
int		is_valid_command(t_token *token, t_mini *ms);
int		execute_pipeline(t_mini *ms);
int		execute_token(t_token *token, t_mini *ms);
char	*find_command_path(char *cmd, t_env *env);
int		execute_simple_command(t_token *token, t_mini *ms);
int		execute_external_command(t_token *token, t_mini *ms);
char	*find_command_path(char *cmd, t_env *env);
int		execute_external_builtin(t_token *token, t_mini *ms);
void	close_all_pipes(t_pipe_ctx *ctx);
int		**create_pipes(int cmd_count);
int		count_commands(t_token *tokens);
void	fill_pipes_cmd(t_token *token);
int		is_append(char *cmd);
void	setup_append_token(t_token *current, t_mini *ms);
int		is_redirect_in(char *cmd);
void	setup_heredoc_in_token(t_token *current, t_mini *ms);
int		is_heredoc(char *cmd);
int		exec_builtin_commands(t_token *token, t_mini *mini, char **paths);
int		exec_system_builtins(t_token *token, t_mini *mini, char **paths);
int		exec_basic_builtins(t_token *token, t_mini *mini);
int		handle_fork_error2(char *cmd_path);
void	print_command_not_found(char *cmd);
int		process_redirection(t_token *current);
int		validate_and_find_command(t_token *token, t_mini *ms, char **cmd_path);
int		execute_fork_process(t_token *token, t_mini *ms, char *cmd_path);
char	*print_echo(char *input, int *i, char *var, t_mini *mini);
t_token	*get_next_command_token(t_token **token_ptr);
int		init_pipeline_context(t_mini *ms, t_pipe_ctx *ctx, pid_t **pids);
void	execute_pipeline_command(t_token *token_list,
			t_pipe_ctx *ctx, t_mini *ms, int type);
char	*join_command_segment(t_token **token);
int		count_pipe_segments(t_token *token);
int		*extract_command_types(t_token *token, int count);
int		check_special_token(t_parser *state);
char	*build_full_path(char *path, char *cmd);
char	*search_in_paths(char **paths, char *cmd, char *path_env);
int		ft_isspace(int c);
char	*ft_strchrnul(const char *s, int c);
void	create_export(char *var, char *content, t_mini *mini);
char	*export_expand(char *input, t_mini *mini);
int		is_directory(const char *path);

#endif
