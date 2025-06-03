/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_validation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:50:15 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/03 16:54:48 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_has_no_arguments(t_token *current)
{
	if (!current->next)
	{
		ft_putstr_fd("expr: missing arguments\n", 2);
		return (1);
	}
	return (0);
}

int	ft_check_args_validity(t_token *current, t_mini *ms)
{
	(void)(ms);
	if (ft_has_no_arguments(current))
	{
		g_exit_status = 1;
		return (-1);
	}
	if (!current->next)
	{
		ft_putstr_fd("expr: missing arguments\n", 2);
		g_exit_status = 1;
		return (-1);
	}
	if (!ft_strcmp(current->next->cmd, "$"))
		return (handle_standalone(current));
	return (1);
}

int	ft_validate_single_arg(t_token *arg_token)
{
	int		i;

	i = 0;
	while (arg_token->cmd[i])
	{
		if (!ft_isdigit(arg_token->cmd[i])
			&& !(i == 0 && (arg_token->cmd[i] == '-'
					|| arg_token->cmd[i] == '+')))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_just_one_arg(t_token *current, t_mini *ms)
{
	t_token	*arg_token;
	int		is_number;

	arg_token = current->next;
	is_number = ft_validate_single_arg(arg_token);
	if (!is_number && ft_strcmp(arg_token->cmd, "$?") != 0)
	{
		if (ms->expr_seen == 1)
		{
		}
		else
		{
			ft_putstr_fd(arg_token->cmd, 1);
			ft_putstr_fd("\n", 1);
		}
		g_exit_status = 0;
		mark_tokens_as_processed(current->next);
		return (-1);
	}
	return (1);
}

int	ft_determine_expression_type(t_token *current, t_mini *ms)
{
	t_token	*second_arg;
	int		is_math_expr;

	is_math_expr = 0;
	if (current->next && !current->next->next)
		is_math_expr = ft_just_one_arg(current, ms);
	if (is_math_expr == -1)
		return (-1);
	if (current->next && current->next->next)
	{
		second_arg = current->next->next;
		if (ft_is_valid_operator_token(second_arg))
			is_math_expr = 1;
		else
		{
			ft_putstr_fd("expr: syntax error: unexpected argument \"", 2);
			ft_putstr_fd(second_arg->cmd, 2);
			ft_putstr_fd("\"\n", 2);
			g_exit_status = 1;
			mark_tokens_as_processed(current->next);
			return (-1);
		}
	}
	return (is_math_expr);
}
