/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:28:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/02 16:56:44 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "./../minishell.h"

int parser(t_parser *state, t_mini *ms)
{
	if (!ms || !ms->input)
		return (0);
	
	if (state->curr)
		ft_free_minishell(ms, 1);
	state->curr = ms->token;
	state->prev = NULL;
	state->last_cmd = NULL;
	state->cmd_seen = 0;
	while (state->curr)
	{
		process_token(state, ms);
		printf("cmd: %s\n", state->curr->cmd);
		printf("type: %d\n", state->curr->type);
		state->prev = state->curr;
		state->curr = state->curr->next;
	}
	
	return (1);
}