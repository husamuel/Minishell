/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:08:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/12 15:38:14 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	exec_exit(t_token *token)
{
	int	i;

	i = 0;
	printf("exit\n");
	if (token->args[1])
	{
		if (token->args[1][0] == '-')
			i++;
		while (token->args[1][i])
		{
			if (!ft_isdigit(token->args[1][i]))
			{
				printf("minishell: exit: %s: "
					"numeric argument required\n", token->args[1]);
				exit(2);
			}
			i++;
		}
		exit(ft_atoi(token->args[1]));
	}
	else
		exit(0);
}
