/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:31:33 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/01 10:12:07 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	setup_cmd_args(t_parser *state)
{
	state->curr->args = malloc(sizeof(char *) * 2);
	if (!state->curr->args)
		return (0);
	state->curr->args[0] = ft_strdup(state->curr->cmd);
	if (!state->curr->args[0])
	{
		free(state->curr->args);
		state->curr->args = NULL;
		return (0);
	}
	state->curr->args[1] = NULL;
	return (1);
}

static int	setup_cmd_args_file(t_parser *state)
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
	return (1);
}

void	setup_command_after_exit_status(t_parser *state)
{
	if (!state->curr || !state->curr->cmd)
		return ;
	state->cmd_seen = 1;
	state->last_cmd = state->curr;
	if (!state->curr->args)
	{
		if (!setup_cmd_args(state))
			return ;
	}
	if (!state->curr->args_file)
	{
		g_exit_status = 127;
		printf("%d: command not found\n", g_exit_status);
		if (!setup_cmd_args_file(state))
			return ;
	}
}

void	ft_decide_on_exit_status(t_parser *state, t_mini *ms)
{
	if (!is_in_expr_context(state->prev))
	{
		process_exit_status(state->curr);
		if (!state->cmd_seen)
			setup_command_after_exit_status(state);
		else if (state->last_cmd)
			handle_arg_token(state, ms);
	}
	else
	{
		ms->exit_status_count++;
		if (state->last_cmd)
			handle_arg_token(state, ms);
	}
}

int	is_valid_for_processing(t_mini *ms)
{
	return (ms->token && ms->token->type != CMD_EXPR
		&& ms->token->type != CMD_REDIRECT_IN
		&& ms->token->type != CMD_REDIRECT_OUT
		&& ms->token->type != CMD_PIPE
		&& ms->token->type != CMD_APPEND);
}
