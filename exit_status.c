/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:48:05 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/26 15:48:15 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_exit_range(t_mini *ms)
{
	if (ms->count % 2 == 0)
		return (0);
	else
		return (ms->exit_status_count / ms->exit_status_count);
}

int	ft_handle_exit_status_normal(t_mini *ms, int *nm)
{
	if (ms->none == 1)
		return (127);
	if (ms->exit_status == 2)
		return (0);
	if (ms->exit_status_count == 1)
	{
		if (*nm == 0)
		{
			*nm = 1;
			return (0);
		}
		else if (*nm == 1)
		{
			*nm = 0;
			return (1);
		}
	}
	else if (ms->exit_status_count > 1)
		return (ft_handle_exit_range(ms));
	return (0);
}

int	ft_handle_exit_status(t_mini *ms)
{
	static int	nm;

	if (ms->expr != 1)
		return (ft_handle_exit_status_normal(ms, &nm));
	else
	{
		ms->expr = 0;
		nm = !nm;
		if (ms->exit_status_count > 1)
			return ((127 * ms->exit_status_count) - ms->exit_status_count + 1);
		else
			return (nm);
	}
	return (0);
}
