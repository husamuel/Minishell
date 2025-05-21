/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:49:31 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/21 13:27:26 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_yet(char *op, t_token **current, int result, t_mini *ms)
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

int	handle_operator(char *op, t_token **current, int result, t_mini *ms)
{
	(*current) = (*current)->next;
	if (!(*current))
	{
		ft_putstr_fd("expr: missing operand after '", 2);
		ft_putstr_fd(op, 2);
		ft_putstr_fd("\n", 2);
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

int	ft_is_valid_operator(char *symbol)
{
	return (
		ft_strcmp(symbol, "+") == 0
		|| ft_strcmp(symbol, "-") == 0
		|| ft_strcmp(symbol, "*") == 0
		|| ft_strcmp(symbol, "/") == 0
		|| ft_strcmp(symbol, "%") == 0);
}

void	ft_handle_unknown_operator(t_token *current)
{
	ft_putstr_fd("expr: unknown operator '", 2);
	ft_putstr_fd(current->cmd, 2);
	ft_putstr_fd("\n", 2);
}

int	ft_is_valid_operator_token(t_token *token)
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
