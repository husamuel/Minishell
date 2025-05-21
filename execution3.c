/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:35:09 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/21 16:52:46 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_child(t_token *cmd, t_mini *ms)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		restore_ctrl_c_only();
		execute_cmd(cmd, ms);
	}
	return (pid);
}
