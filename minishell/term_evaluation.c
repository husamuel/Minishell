/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_evaluation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:01:15 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/01 10:25:21 by husamuel         ###   ########.fr       */
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
	(void)(ms);
	if (ft_strcmp((char *)term, "$?") == 0)
		return (g_exit_status);
	if (!ft_validate_numeric_term(term))
		return (0);
	return (ft_atoi(term));
}
