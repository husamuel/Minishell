/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:50:15 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/21 19:13:01 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_handle_exit_range(t_mini *ms)
{
	if (ms->count % 2 == 0)
		return (0);
	else
		return (ms->exit_status_count / ms->exit_status_count);
}

static int	ft_handle_exit_status(t_mini *ms)
{
	static int	nm;

	nm = 0;
	if (ms->none == 1)
		return (127);
	if (ms->exit_status == 2)
		return (0);
	if (ms->exit_status_count == 1)
	{
		if (nm == 0)
		{
			nm = 1;
			return (0);
		}
		else if (nm == 1)
		{
			nm = 0;
			return (1);
		}
	}
	else if (ms->exit_status_count > 1)
		return (ft_handle_exit_range(ms));
	return (0);
}

static int	ft_validate_numeric_term(const char *term)
{
	int	i;

	i = 0;
	while (term[i])
	{
		if (!ft_isdigit(term[i]))
		{
			if (ft_strcmp((char *)term, "$?") != 0)
			{
				ft_putstr_fd("expr: invalid value: '", 2);
				ft_putstr_fd((char *)term, 2);
				ft_putstr_fd("'\n", 2);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	evaluate_term(const char *term, t_mini *ms)
{
	if (ft_strcmp((char *)term, "$?") == 0)
		return (ft_handle_exit_status(ms));
	if (!ft_validate_numeric_term(term))
		return (0);
	return (ft_atoi(term));
}

static int	ft_handle_yet(char *op, t_token **current, int result, t_mini *ms)
{
	int	divisor;

	divisor = evaluate_term((*current)->cmd, ms);
	if (divisor == 0)
	{
		ft_putstr_fd("expr: ", 2);
		if (op[0] == '/')
			ft_putstr_fd("division", 2);
		else
			ft_putstr_fd("modulo", 2);
		ft_putstr_fd(" by zero\n", 2);
		return (0);
	}
	if (op[0] == '/')
		return (result / divisor);
	else
		return (result % divisor);
}

static int	handle_operator(char *op, t_token **current, int result, t_mini *ms)
{
	(*current) = (*current)->next;
	if (!(*current))
	{
		ft_putstr_fd("expr: missing operand after '", 2);
		ft_putstr_fd(op, 2);
		ft_putstr_fd("'\n", 2);
		return (result);
	}
	if (ft_strcmp(op, "+") == 0)
		return (result + evaluate_term((*current)->cmd, ms));
	else if (ft_strcmp(op, "-") == 0)
		return (result - evaluate_term((*current)->cmd, ms));
	else if (ft_strcmp(op, "*") == 0)
		return (result * evaluate_term((*current)->cmd, ms));
	else if (ft_strcmp(op, "/") == 0 || ft_strcmp(op, "%") == 0)
		return (ft_handle_yet(op, current, result, ms));
	return (result);
}

static int	ft_is_valid_operator(char *operator)
{
	return (
		ft_strcmp(operator, "+") == 0
		|| ft_strcmp(operator, "-") == 0
		|| ft_strcmp(operator, "*") == 0
		|| ft_strcmp(operator, "/") == 0
		|| ft_strcmp(operator, "%") == 0);
}

static void	ft_handle_unknown_operator(t_token *current)
{
	ft_putstr_fd("expr: unknown operator '", 2);
	ft_putstr_fd(current->cmd, 2);
	ft_putstr_fd("'\n", 2);
}

static int	process_expression(t_token *tokens, t_mini *ms)
{
	int		result;
	t_token	*current;
	char	*operator;

	result = 0;
	current = tokens;
	if (current)
	{
		result = evaluate_term(current->cmd, ms);
		current = current->next;
	}
	while (current)
	{
		operator = current->cmd;
		if (ft_is_valid_operator(operator))
			result = handle_operator(operator, &current, result, ms);
		else
		{
			ft_handle_unknown_operator(current);
			break ;
		}
		current = current->next;
	}
	return (result);
}

static void	mark_tokens_as_processed(t_token *start)
{
	t_token	*current;

	current = start;
	while (current)
	{
		current->type = CMD_NONE;
		current = current->next;
	}
}

static int	ft_is_valid_operator_token(t_token *token)
{
	if (!token)
		return (0);
	return (
		ft_strcmp(token->cmd, "+") == 0
		|| ft_strcmp(token->cmd, "-") == 0
		|| ft_strcmp(token->cmd, "*") == 0
		|| ft_strcmp(token->cmd, "/") == 0
		|| ft_strcmp(token->cmd, "%") == 0);
}

static int	ft_has_no_arguments(t_token *current)
{
	if (!current->next)
	{
		ft_putstr_fd("expr: missing arguments\n", 2);
		return (1);
	}
	return (0);
}

void	process_expr_command(t_token *current, t_mini *ms)
{
	int		result;
	int		is_math_expr;
	
	if (ft_has_no_arguments(current))
	{
		ms->exit_status = 1;
		return ;
	}

	int		i;
	int		is_number;
	t_token	*arg_token;
	t_token	*second_arg;

	is_math_expr = 0;
	if (!current->next)
	{
		ft_putstr_fd("expr: missing arguments\n", 2);
		ms->exit_status = 1;
		return ;
	}
	if (current->next && !current->next->next)
	{
		arg_token = current->next;
		i = 0;
		is_number = 1;
		while (arg_token->cmd[i])
		{
			if (!ft_isdigit(arg_token->cmd[i])
				&& !(i == 0 && (arg_token->cmd[i] == '-'
						|| arg_token->cmd[i] == '+')))
			{
				is_number = 0;
				break ;
			}
			i++;
		}
		if (!is_number && ft_strcmp(arg_token->cmd, "$?") != 0)
		{
			ft_putstr_fd(arg_token->cmd, 1);
			ft_putstr_fd("\n", 1);
			ms->exit_status = 0;
			mark_tokens_as_processed(current->next);
			return ;
		}
		is_math_expr = 1;
	}
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
			ms->exit_status = 1;
			mark_tokens_as_processed(current->next);
			return ;
		}
	}
	if (is_math_expr)
	{
		result = process_expression(current->next, ms);
		ft_putnbr_fd(result, 1);
		ft_putstr_fd("\n", 1);
		ms->exit_status = result;
	}
	else
	{
		result = process_expression(current->next, ms);
		ft_putnbr_fd(result, 1);
		ft_putstr_fd("\n", 1);
		ms->exit_status = result;
	}
	mark_tokens_as_processed(current->next);
}

void	expr_verify(t_token *token)
{
	if (!token || !token->cmd)
		ft_putstr_fd("expr: invalid expression\n", 2);
}
