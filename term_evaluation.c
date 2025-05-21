/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_evaluation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:01:15 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/21 13:27:38 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_validate_numeric_term(const char *term)
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
				ft_putstr_fd("\n", 2);
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
/*
void	expr_verify(t_token *token)
{
	if (!token || !token->cmd)
		ft_putstr_fd("expr: invalid expression\n", 2);
}*/
