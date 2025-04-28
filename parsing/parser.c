/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:28:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/28 12:29:12 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	parser(t_mini *ms)
{
	t_parser	state;

	if (ms->input == NULL)
		return (0);
	state.curr = ms->token;
	state.prev = NULL;
	state.last_cmd = NULL;
	state.cmd_seen = 0;
	while (state.curr)
	{
		process_token(&state, ms);
		state.prev = state.curr;
		state.curr = state.curr->next;
	}
	return (1);
}
