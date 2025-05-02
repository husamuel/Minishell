/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/02 17:13:51 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_minishell(t_mini *ms, int code)
{
	while (ms->state->curr)
	{
		if (ms->state->curr->args)
		{
			free(ms->state->curr->args[0]);
			free(ms->state->curr->args);
		}
		if (ms->state->curr->args_file)
		{
			free(ms->state->curr->args_file[0]);
			free(ms->state->curr->args_file);
		}
		if (code != 100)
			break ;
		ms->state->curr = ms->state->curr->next;
	}
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;
	int		i;

	while (token)
	{
		tmp = token->next;
		if (token->cmd)
			free(token->cmd);
		if (token->args_file)
		{
			i = 0;
			while (token->args_file[i])
			{
				free(token->args_file[i]);
				i++;
			}
			free(token->args_file);
		}
		if (token->args)
		{
			i = 0;
			while (token->args[i])
			{
				free(token->args[i]);
				i++;
			}
			free(token->args);
		}
		free(token);
		token = tmp;
	}
}
