/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:21:32 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/04 13:36:16 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	exec_end(t_mini *ms)
{
	t_token	*current;
	int		marker;

	marker = 0;
	current = ms->token;
	while (current)
	{
		if (is_valid_command(current, ms))
		{
			restore_child_tty();
			g_exit_status = execute_token(current, ms);
			marker = 1;
		}
		current = current->next;
	}
	if (!marker)
		g_exit_status = 127;
}

void	exec(t_mini *ms)
{
	if (!ms || !ms->token)
	{
		g_exit_status = 0;
		return ;
	}
	setup_exec_signals();
	if (ms->pipe > 0)
	{
		g_exit_status = execute_pipeline(ms);
	}
	else
		exec_end(ms);
}
