/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:02:04 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/21 12:31:33 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_expression(t_token *tokens, t_mini *ms)
{
	int		result;
	t_token	*current;
	char	*symbol;

	result = 0;
	current = tokens;
	if (current)
	{
		result = evaluate_term(current->cmd, ms);
		current = current->next;
	}
	while (current)
	{
		symbol = current->cmd;
		if (ft_is_valid_operator(symbol))
			result = handle_operator(symbol, &current, result, ms);
		else
		{
			ft_handle_unknown_operator(current);
			break ;
		}
		current = current->next;
	}
	return (result);
}

void	mark_tokens_as_processed(t_token *start)
{
	t_token	*current;

	current = start;
	while (current)
	{
		current->type = CMD_NONE;
		current = current->next;
	}
}

void	process_expr_command(t_token *current, t_mini *ms)
{
	int		result;
	int		is_math_expr;

	ms->expr_seen = 1;
	if (ft_check_args_validity(current, ms))
		return ;
	is_math_expr = ft_determine_expression_type(current, ms);
	if (is_math_expr == -1)
		return ;
	result = process_expression(current->next, ms);
	ft_putnbr_fd(result, 1);
	ft_putstr_fd("\n", 2);
	ms->exit_status = result;
	mark_tokens_as_processed(current->next);
}
