/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/21 12:26:22 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		free(token);
		token = tmp;
	}
}
