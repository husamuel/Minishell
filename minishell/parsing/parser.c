/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:28:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/03 15:45:01 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	parser(t_parser *state, t_mini *ms)
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
		state->prev = state->curr;
		state->curr = state->curr->next;
	}
	fill_pipes_cmd(ms->token);
	return (1);
}
