/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:58:13 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/04 13:34:19 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	allocate_cmd_args(t_parser *state)
{
	state->curr->args_file = malloc(sizeof(char *) * 2);
	if (!state->curr->args_file)
		return (0);
	state->curr->args_file[0] = NULL;
	state->curr->args_file[1] = NULL;
	state->curr->args = malloc(sizeof(char *) * 2);
	if (!state->curr->args)
	{
		free(state->curr->args_file);
		state->curr->args_file = NULL;
		return (0);
	}
	state->curr->args[0] = NULL;
	state->curr->args[1] = NULL;
	return (1);
}

static	int	set_cmd_args(t_parser *state)
{
	if (!state->curr->args[0])
	{
		state->curr->args[0] = ft_strdup(state->curr->cmd);
		if (!state->curr->args[0])
		{
			free(state->curr->args);
			state->curr->args = NULL;
			return (0);
		}
	}
	if (!state->curr->args_file[0])
	{
		state->curr->args_file[0] = ft_strdup(state->curr->cmd);
		if (!state->curr->args_file[0])
		{
			free(state->curr->args_file);
			state->curr->args_file = NULL;
			return (0);
		}
	}
	return (1);
}

static int	handle_redirect_argument(t_parser *state)
{
	if (state->prev && state->curr->type == CMD_NONE
		&& (state->prev->type == CMD_HEREDOC
			|| state->prev->type == CMD_REDIRECT_OUT
			|| state->prev->type == CMD_REDIRECT_IN
			|| state->prev->type == CMD_APPEND))
	{
		state->curr->type = CMD_ARG_FILE;
		return (1);
	}
	return (0);
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
	else if (handle_redirect_argument(state))
		return ;
	else if (check_special_token(state))
	{
		if (!set_cmd_args(state))
			return ;
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
			|| is_redirect_out(state->prev->cmd)
			|| is_append(state->prev->cmd)))
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
