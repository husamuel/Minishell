/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:09:09 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/02 17:11:08 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	display_command_not_found(t_token *token, t_mini *ms)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(token->cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	ms->exit_status = 127;
	ms->expr = 1;
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

static int	allocate_cmd_args(t_parser *state)
{
	state->curr->args_file = malloc(sizeof(char *) * 2);
	if (!state->curr->args_file)
		return (0);
	state->curr->args_file[0] = ft_strdup(state->curr->cmd);
	if (!state->curr->args_file[0])
	{
		free(state->curr->args_file);
		state->curr->args_file = NULL;
		return (0);
	}
	state->curr->args_file[1] = NULL;
	state->curr->args = malloc(sizeof(char *) * 2);
	if (!state->curr->args)
	{
		free(state->curr->args_file[0]);
		free(state->curr->args_file);
		state->curr->args_file = NULL;
		return (0);
	}
	return (1);
}

static int	set_cmd_args(t_parser *state)
{
	state->curr->args[0] = ft_strdup(state->curr->cmd);
	if (!state->curr->args[0])
	{
		free(state->curr->args_file[0]);
		free(state->curr->args_file);
		state->curr->args_file = NULL;
		free(state->curr->args);
		state->curr->args = NULL;
		return (0);
	}
	state->curr->args[1] = NULL;
	return (1);
}

static int	check_special_token(t_parser *state)
{
	return (state->curr->type == CMD_PIPE
		|| state->curr->type == CMD_REDIRECT
		|| state->curr->type == CMD_REDIRECT_IN
		|| state->curr->type == CMD_REDIRECT_OUT
		|| state->curr->type == CMD_ARG_FILE
		|| state->curr->type == CMD_ARG
		|| state->curr->type == CMD_HEREDOC);
}

void	handle_command_token(t_parser *state, t_mini *ms)
{
	if (!allocate_cmd_args(state))
		return ;
	if (!set_cmd_args(state))
		return ;
	if (is_builtin_command(state->curr->cmd)
		|| is_exec_command(state->curr->cmd))
	{
		set_command_type(state->curr);
		state->cmd_seen = 1;
		state->last_cmd = state->curr;
	}
	else if (check_special_token(state) && state->prev != NULL)
	{
		/* Do nothing for special tokens with a previous token */
	}
	else
	{
		ms->none = 1;
		display_command_not_found(state->curr, ms);
	}
}

void	handle_arg_token(t_parser *state, t_mini *ms)
{
	(void)ms;
	if (!state->last_cmd || !state->curr || !state->curr->cmd)
		return ;
	if (state->prev && (is_redirect_in(state->prev->cmd)
			|| is_redirect_out(state->prev->cmd)))
	{
		add_to_args_file(state->last_cmd, state->curr->cmd);
		state->curr->type = CMD_ARG_FILE;
	}
	else
	{
		add_to_args(state->last_cmd, state->curr->cmd);
		state->curr->type = CMD_ARG;
	}
}
