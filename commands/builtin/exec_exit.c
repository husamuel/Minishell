/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:08:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/20 18:11:09 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	exec_exit(t_token *token, t_mini *ms)
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
				ms->exit_status = 2;
				exit(ms->exit_status);
			}
			i++;
		}
		ms->exit_status = ft_atoi(token->args[1]);
		exit(ms->exit_status);
	}
	else
		exit(ms->exit_status = 0);
}
