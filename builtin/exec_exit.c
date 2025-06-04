/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:08:56 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/04 12:10:09 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static unsigned long	ft_atoll(const char *str)
{
	unsigned long	n;
	unsigned int	i;

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

static void	ft_spec_exit(t_token *token)
{
	if (!ft_strncmp(token->args[1], "-9223372036854775808", 21))
	{
		if (token->args[2])
		{
			printf("minishell: exit: too many arguments\n");
			g_exit_status = 1;
		}
		else
			g_exit_status = 0;
	}
	else
	{
		printf("minishell: exit: %s: numeric argument required\n",
			token->args[1]);
		g_exit_status = 2;
	}
	setup_signals();
	exit(g_exit_status);
}

void	ft_last_exit_check(t_token *token)
{
	if (token->args[2])
	{
		printf("minishell: exit: too many arguments\n");
		g_exit_status = 1;
	}
	else
		g_exit_status = ft_atoi(token->args[1]);
	setup_signals();
	exit(g_exit_status);
}

void	handle_exit_arguments(t_token *token)
{
	unsigned long	num;
	unsigned int	i;

	i = 0;
	if (!ft_strncmp("-9223372036854775808", token->args[1], 21))
		ft_spec_exit(token);
	if (ft_strlen(token->args[1]) > 20)
		ft_spec_exit(token);
	num = ft_atoll(token->args[1] + i);
	while (token->args[1][i])
	{
		if (token->args[1][i] == '+' || token->args[1][i] == '-')
			i++;
		if (!ft_isdigit(token->args[1][i]) || num > 9223372036854775807)
			ft_spec_exit(token);
		i++;
	}
	ft_last_exit_check(token);
}

void	exec_exit(t_token *token)
{
	printf("exit\n");
	if (token->args[1])
		handle_exit_arguments(token);
	else
	{
		g_exit_status = 0;
		setup_signals();
		exit(g_exit_status);
	}
}

/*
we need to check the shell level and for each of them setup_signals();
static int	get_shell_level()
{
	char	*shlvl;
	int	level;

	level = 0;
	shlvl = getenv("SHLVL");
	if (shlvl)
		level = ft_atoi(shlvl);
	else
		level = 1;
	printf("%d\n", level);
	return (level);

}*/
