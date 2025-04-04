#include "minishell.h"

void	add_to_args(t_token *token, char *arg)
{
	int		i;
	int		j;
	char	**new_args;

	i = 0;
	if (token->args)
	{
		while (token->args[i])
			i++;
	}
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	if (token->args)
	{
		j = 0;
		while (j < i)
		{
			new_args[j] = token->args[j];
			j++;
		}
		free(token->args);
	}
	new_args[i] = strdup(arg);
	new_args[i + 1] = NULL;
	token->args = new_args;
}

void	add_to_args_file(t_token *token, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	while (token->args_file[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	ft_memcpy(new_args, token->args_file, sizeof(char *) * i);
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(token->args_file);
	token->args_file = new_args;
}

static void	handle_command_token(t_token *current, t_token **last_cmd, int *command_seen)
{
	current->args_file = malloc(sizeof(char *) * 2);
	if (!current->args_file)
		return ;
	current->args_file[0] = strdup(current->cmd);
	current->args_file[1] = NULL;
	current->args = malloc(sizeof(char *) * 2);
	if (!current->args)
		return ;
	current->args[0] = strdup(current->cmd);
	current->args[1] = NULL;
	if (ft_strcmp(current->cmd, "$?") == 0)
		current->type = CMD_EXIT_STATUS;
	else if (is_builtin_command(current->cmd) || is_exec_command(current->cmd))
	{
		set_command_type(current);
		*command_seen = 1;
		*last_cmd = current;
	}
	else
		current->type = CMD_NONE;
}

static void	handle_argument_token(t_token *current, t_token *prev,
								t_token *last_cmd, t_mini *ms)
{
	char	*processed_arg;

	(void)prev;
	(void)ms;
	if (!last_cmd)
		return ;
	processed_arg = strdup(current->cmd);
	if (processed_arg)
	{
		add_to_args_file(last_cmd, processed_arg);
		add_to_args(last_cmd, processed_arg);
		current->type = CMD_ARG;
	}
}

void	process_token(t_token *current, t_token *prev, t_token **last_cmd,
					int *command_seen, t_mini *ms)
{
	if(ft_strcmp(current->cmd, "./minishell") == 0)
	{
		current->type = CMD_SUBSHELL;
		current->args_file = malloc(sizeof(char *) * 2);
		if (!current->args_file)
			return ;
		current->args_file[0] = strdup("./minishell");
		current->args_file[1] = NULL;
		return ;
	}

	if (ft_strcmp(current->cmd, "expr") == 0)
	{
		current->type = CMD_EXPR;
		*command_seen = 1;
		*last_cmd = current;
		current->args_file = malloc(sizeof(char *) * 2);
		if (!current->args_file)
			return ;
		current->args_file[0] = strdup(current->cmd);
		current->args_file[1] = NULL;
		current->args = malloc(sizeof(char *) * 2);
		if (!current->args)
			return ;
		current->args[0] = strdup(current->cmd);
		current->args[1] = NULL;
		return ;
	}
	if (current->cmd[0] == '|')
	{
		current->type = CMD_PIPE;
		++ms->pipe;
	}
	else if (current->cmd[0] == '\\' || current->cmd[0] == ';')
		current->type = CMD_NONE;
	else if (!(*command_seen))
		handle_command_token(current, last_cmd, command_seen);
	else
		handle_argument_token(current, prev, *last_cmd, ms);
}