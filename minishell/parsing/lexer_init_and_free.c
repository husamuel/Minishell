/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init_and_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:28:34 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 15:41:18 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	ft_init_lexer_state(t_lexer *state, char *input)
{
	state->head = NULL;
	state->tail = NULL;
	state->in_quotes = 0;
	state->quote_type = '\0';
	state->pos[0] = 0;
	state->pos[1] = 0;
	state->input = input;
}

t_token	*create_new_token(char *cmd)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	ft_memset(new_token, 0, sizeof(t_token));
	if (cmd)
		new_token->cmd = ft_strdup(cmd);
	else
		new_token->cmd = NULL;
	return (new_token);
}

void	free_token_list(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->cmd);
		free(temp);
	}
}

char	**free_mat(char **mat)
{
	int	i;

	if (!mat)
		return (NULL);
	i = 0;
	while (mat[i])
	{
		free(mat[i]);
		i++;
	}
	free(mat);
	return (NULL);
}
