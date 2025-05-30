/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_char.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:33:22 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 17:39:05 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	handle_spaces(t_lexer *state)
{
	int	i;

	i = state->pos[1];
	if (!(state->in_quotes) && ft_isspace(state->input[i]) != 0)
	{
		if (i > state->pos[0])
		{
			state->pos[1] = i;
			if (!add_previous_token(state))
				return (0);
		}
		state->pos[0] = i + 1;
	}
	return (1);
}

static int	handle_quote(t_lexer *state, char current_char)
{
	if (current_char == '"' || current_char == '\'')
	{
		if (!(state->in_quotes))
		{
			state->in_quotes = 1;
			state->quote_type = current_char;
		}
		else if (current_char == state->quote_type)
		{
			state->in_quotes = 0;
			state->quote_type = '\0';
		}
	}
	return (1);
}

static int	handle_operators(t_lexer *state)
{
	int	i;

	i = state->pos[1];
	if (!state->in_quotes && (state->input[i] == '|'
			|| state->input[i] == '<' || state->input[i] == '>'))
	{
		if (i > state->pos[0])
		{
			state->pos[1] = i;
			if (!add_previous_token(state))
				return (0);
		}
		if (!create_operator_token(state, &i))
			return (0);
		state->pos[0] = i + 1;
		state->pos[1] = i;
	}
	return (1);
}

int	process_char(t_lexer *state)
{
	int	i;

	i = state->pos[1];
	if (!handle_spaces(state))
		return (0);
	if (!handle_quote(state, state->input[i]))
		return (0);
	if (!handle_operators(state))
		return (0);
	return (1);
}
