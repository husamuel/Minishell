/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:33:36 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/23 17:33:46 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	exec_builtin_commands(t_token *token, t_mini *mini, char **paths)
{
	int	status;

	status = exec_basic_builtins(token, mini);
	if (status != 127)
		return (status);
	return (exec_system_builtins(token, mini, paths));
}

int	exec_builtin(t_token *token, t_mini *mini)
{
	int		status;
	char	*system_paths[3];

	status = 127;
	system_paths[0] = "/bin/echo";
	system_paths[1] = "/bin/pwd";
	system_paths[2] = "/bin/env";
	if (!is_builtin(token->cmd))
		return (status);
	status = exec_builtin_commands(token, mini, system_paths);
	return (status);
}
