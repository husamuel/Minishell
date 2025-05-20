/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:49:23 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 20:53:54 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_special_addition_case(t_token *current, t_token *prev,
		t_mini *ms)
{
	if (ft_strcmp(current->cmd, "$?+$?") == 0
		&& prev
		&& (prev->type == CMD_EXPR || prev->type == CMD_BUILDIN))
	{
		printf("%d+%d\n", ms->exit_status, ms->exit_status);
		return (1);
	}
	return (0);
}

static int	handle_special_token_case(t_token **current,
		t_token **prev, t_mini *ms)
{
	t_token	*special_token;
	t_token	*temp;

	if ((*current)->type == CMD_BUILDIN || (*current)->type == CMD_EXEC)
	{
		if (ms->pipe == 0)
		{
			special_token = find_special_token(*current);
			if (special_token)
			{
				process_special_token(special_token, ms);
				temp = (*current)->next;
				while (*current && *current != temp)
					*current = (*current)->next;
				if (*current)
					*prev = (*current)->prev;
				else
					*prev = NULL;
				return (1);
			}
		}
	}
	return (0);
}

void	ft_exec_token_list(t_mini *ms)
{
	t_token	*current;
	t_token	*prev;

	current = ms->token;
	prev = NULL;
	while (current)
	{
		if (is_special_addition_case(current, prev, ms))
		{
			prev = current;
			current = current->next;
			continue ;
		}
		if (handle_special_token_case(&current, &prev, ms))
			continue ;
		if (ft_handle_token(current, prev, ms))
			return ;
		prev = current;
		current = current->next;
	}
}
