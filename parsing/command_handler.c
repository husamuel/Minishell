/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:09:09 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/03 11:36:21 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	display_command_not_found(t_token *token, t_mini *ms)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(token->cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	g_exit_status = 127;
	ms->expr = 1;
}

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

void	set_command_type(t_token *current)
{
	if (is_builtin_command(current->cmd))
		current->type = CMD_BUILDIN;
	else if (ft_strcmp(current->cmd, "./minishell") == 0)
	{
		current->type = CMD_EXEC;
	}
	else if (is_exec_command(current->cmd))
		current->type = CMD_EXEC;
}
/*	-> update shlvl like in export */
