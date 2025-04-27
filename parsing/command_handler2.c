/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:41:40 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 18:55:16 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	setup_command_after_exit_status(t_token *current, int *command_seen,
	t_token **last_cmd) //TODO
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

void	ft_handle_spec(t_token *curr, int *cmd_seen, t_token **last, t_mini *ms) //TODO
{
	if (ft_strcmp(curr->cmd, "expr") == 0)
	{
		setup_expr_command(curr, cmd_seen, last);
		return ;
	}
	if (curr->cmd[0] == '|')
		setup_pipe_token(curr, ms);
	else if (is_redirect_out(curr->cmd))
		setup_redirect_out_token(curr, ms);
	else if (is_redirect_in(curr->cmd))
		setup_redirect_in_token(curr, ms);
	else if (curr->cmd[0] == '\\' || curr->cmd[0] == ';')
		curr->type = CMD_NONE;
}

void	ft_handle_norm(t_token *curr, t_token *prev, t_token *last, t_mini *ms) //TODO
{
	char	*processed_arg;

	if (prev && (prev->type == CMD_REDIRECT_IN
			|| prev->type == CMD_REDIRECT_OUT
			|| prev->type == CMD_HEREDOC))
	{
		processed_arg = ft_strdup(curr->cmd);
		if (processed_arg)
		{
			add_to_args_file(last, processed_arg);
			curr->type = CMD_ARG_FILE;
			free(processed_arg);
		}
	}
	else
		handle_arg_token(curr, prev, last, ms);
}

void	process_token_part2(t_token *current, t_token *prev,
		t_token **last_cmd, int *command_seen, t_mini *ms) //TODO
{
	ft_handle_spec(current, command_seen, last_cmd, ms);
	if (!(*command_seen))
		handle_command_token(current, last_cmd, command_seen, ms);
	else
		ft_handle_norm(current, prev, *last_cmd, ms);
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
