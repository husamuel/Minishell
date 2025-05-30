/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:08:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/27 16:24:08 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static unsigned long long	ft_atoll(const char *str)
{
	unsigned long long	n;
	unsigned int		i;

	i = 0;
	n = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == 43 || str[i] == 45)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		n *= 10;
		n = n + str[i] - '0';
		i++;
	}
	return (n);
}

static void	ft_spec_exit(t_mini *ms, t_token *token)
{
	if (!ft_strncmp(token->args[1], "-9223372036854775808", 21))
		ms->exit_status = 0;
	else
	{
		printf("minishell: exit: %s: "
			"numeric argument required\n", token->args[1]);
		ms->exit_status = 2;
	}
	exit(ms->exit_status);
}

void	exec_exit(t_token *token, t_mini *ms)
{
	unsigned long long	num;
	unsigned int		i;

	i = 0;
	printf("exit\n");
	if (token->args[1])
	{
		if (token->args[1][0] == '-')
			i++;
		if (!ft_strncmp("-9223372036854775808", token->args[1], 21))
			ft_spec_exit(ms, token);
		num = ft_atoll(token->args[1] + i);
		while (token->args[1][i])
		{
			if (!ft_isdigit(token->args[1][i])
				|| num > 9223372036854775807)
				ft_spec_exit(ms, token);
			i++;
		}
		ms->exit_status = ft_atoi(token->args[1]);
		exit(ms->exit_status);
	}
	else
		exit(ms->exit_status = 0);
}
