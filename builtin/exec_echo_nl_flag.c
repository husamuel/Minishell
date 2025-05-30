/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo_nl_flag.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:17:16 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 12:43:06 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	check_nl_echo(char *s)
{
	int	i;

	i = 0;
	if (!s || s[i] != '-' || s[i + 1] != 'n')
		return (0);
	i++;
	while (s[i] == 'n')
		i++;
	if (s[i] == '\0')
		return (1);
	return (0);
}

void	ft_handle_flag(t_token **next, int *nl_flag)
{
	*nl_flag = 0;
	while (*next && ft_strncmp((*next)->cmd, "-n", 2) == 0
		&& check_nl_echo((*next)->cmd))
	{
		*next = (*next)->next;
		*nl_flag = 1;
	}
}

int	ft_is_valid_n_flag(char *s, int *len)
{
	int	i;

	i = 0;
	if (s[i] != '-')
		return (0);
	i++;
	if (s[i] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	if (s[i] == '\0' || ft_isspace(s[i]) != 0)
	{
		if (len)
			*len = i;
		return (1);
	}
	return (0);
}

int	ft_skip_n_flags(char *input, int *nl_flag)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	*nl_flag = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]) != 0)
			i++;
		if (ft_is_valid_n_flag(&input[i], &len))
		{
			i += len;
			*nl_flag = 1;
		}
		else
			break ;
	}
	while (ft_isspace(input[i]) != 0)
		i++;
	return (i);
}
