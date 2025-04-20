#include "./../minishell.h"

void	setup_command_after_exit_status(t_token *current, int *command_seen,
	t_token **last_cmd)
{
	*command_seen = 1;
	*last_cmd = current;
	if (!current->args)
	{
		current->args = malloc(sizeof(char *) * 2);
		if (current->args)
		{
			current->args[0] = ft_strdup(current->cmd);
			current->args[1] = NULL;
		}
	}
	if (!current->args_file)
	{
		current->args_file = malloc(sizeof(char *) * 2);
		if (current->args_file)
		{
			current->args_file[0] = ft_strdup(current->cmd);
			current->args_file[1] = NULL;
		}
	}
}

void	process_token_part2(t_token *current, t_token *prev,
	t_token **last_cmd, int *command_seen, t_mini *ms)
{
	if (ft_strcmp(current->cmd, "expr") == 0)
	{
		setup_expr_command(current, command_seen, last_cmd);
		return ;
	}
	if (current->cmd[0] == '|')
		setup_pipe_token(current, ms);
	else if (is_redirect_out(current->cmd))
		setup_redirect_out_token(current, ms);
	else if (is_redirect_in(current->cmd))
		setup_redirect_in_token(current, ms);
	else if (current->cmd[0] == '\\' || current->cmd[0] == ';')
		current->type = CMD_NONE;
	else if (!(*command_seen))
		handle_command_token(current, last_cmd, command_seen, ms);
	else
		handle_argument_token(current, prev, *last_cmd, ms);
}

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

void	set_command_type(t_token *current)
{
	if (is_builtin_command(current->cmd))
		current->type = CMD_BUILDIN;
	else if (is_exec_command(current->cmd))
		current->type = CMD_EXEC;
}

