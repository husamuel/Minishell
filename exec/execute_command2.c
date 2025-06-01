/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:41:56 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/01 10:14:26 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	handle_fork_error2(char *cmd_path)
{
	perror("fork");
	free(cmd_path);
	return (1);
}

void	print_command_not_found(char *cmd)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
}

int	execute_external_command(t_token *token, t_mini *ms)
{
	char	*cmd_path;
	int		validation_result;

	validation_result = validate_and_find_command(token, ms, &cmd_path);
	if (validation_result != 0)
	{
		g_exit_status = validation_result;
		return (validation_result);
	}
	g_exit_status = execute_fork_process(token, ms, cmd_path);
	setup_signals();
	disable_ctrl_backslash();
	return (g_exit_status);
}
