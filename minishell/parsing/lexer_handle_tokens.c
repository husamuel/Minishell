/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:35:57 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 17:38:18 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	append_token(t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		new_token->prev = *tail;
		*tail = new_token;
	}
}

static t_token	*create_token_from_substr(char *input, int start, int end)
{
	char	*token_str;
	t_token	*new_token;

	token_str = ft_substr(input, start, end - start);
	new_token = create_new_token(token_str);
	free(token_str);
	return (new_token);
}

t_token	*add_previous_token(t_lexer *state)
{
	t_token	*new_token;
	int		start;
	int		end;

	start = state->pos[0];
	end = state->pos[1];
	if (end <= start)
		return (NULL);
	new_token = create_token_from_substr(state->input, start, end);
	if (!new_token)
	{
		free_token_list(state->head);
		return (NULL);
	}
	append_token(&state->head, &state->tail, new_token);
	return (new_token);
}

int	create_operator_token(t_lexer *state, int *i)
{
	char	op_str[3];
	t_token	*new_token;

	op_str[0] = state->input[*i];
	op_str[1] = 0;
	op_str[2] = 0;
	if ((state->input[*i] == '>' || state->input[*i] == '<')
		&& state->input[*i + 1] == state->input[*i])
	{
		op_str[1] = state->input[*i];
		(*i)++;
	}
	new_token = create_new_token(op_str);
	if (!new_token)
	{
		free_token_list(state->head);
		return (0);
	}
	append_token(&state->head, &state->tail, new_token);
	return (1);
}

t_token	*lexer(char *input)
{
	t_lexer	state;

	ft_init_lexer_state(&state, input);
	while (state.input[state.pos[1]])
	{
		if (!process_char(&state))
			return (NULL);
		state.pos[1]++;
	}
	if (state.pos[1] > state.pos[0])
	{
		if (!add_previous_token(&state))
			return (NULL);
	}
	return (state.head);
}
