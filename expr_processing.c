/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:02:04 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/03 16:54:36 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_expression_loop(t_token *current, int result, t_mini *ms)
{
	char	*symbol;

	while (current)
	{
		symbol = current->cmd;
		if (ft_is_valid_operator(symbol))
		{
			if (current->next == NULL)
			{
				printf("expr: missing operand after '%s'\n", symbol);
				break ;
			}
			result = handle_operator(symbol, &current, result, ms);
		}
		else
		{
			ft_handle_unknown_operator(current);
			break ;
		}
		current = current->next;
	}
	return (result);
}

int	process_expression(t_token *tokens, t_mini *ms)
{
	int		result;
	t_token	*current;

	result = 0;
	current = tokens;
	if (current)
	{
		result = evaluate_term(current->cmd, ms);
		current = current->next;
	}
	return (process_expression_loop(current, result, ms));
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

int	process_expr_command(t_token *current, t_mini *ms)
{
	int	result;
	int	is_math_expr;
	int	status;

	ms->expr_seen = 1;
	status = ft_check_args_validity(current, ms);
	if (status < 0)
		return (1);
	else if (status == 0)
		return (0);
	is_math_expr = ft_determine_expression_type(current, ms);
	if (is_math_expr == -1)
		return (1);
	result = process_expression(current->next, ms);
	ft_putnbr_fd(result, 1);
	ft_putstr_fd("\n", 2);
	g_exit_status = result;
	mark_tokens_as_processed(current->next);
	return (0);
}

int	handle_standalone(t_token *current)
{
	if (!current->next->next)
	{
		printf("$\n");
		return (0);
	}
	else
	{
		if (!ft_strcmp(current->next->next->cmd, "+")
			&& !current->next->next->next)
			printf("bash: syntax error near "
				"unexpected token `newline'\n");
		else
			printf("expr: non-integer argument\n");
		return (-1);
	}
}
