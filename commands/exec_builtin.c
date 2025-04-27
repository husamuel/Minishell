/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:30:36 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 13:37:30 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	is_builtin(const char *cmd)
{
	int					i;
	static const char	*builtins[] = {
		"cd", "echo", "pwd", "export", "unset", "env", "exit",
		"/bin/pwd", "/bin/env", "/bin/echo", NULL
	};

	if (!cmd)
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(t_token *token, t_mini *mini)
{
	int		status;
	char	*s[3];

	status = 127;
	s[0] = "/bin/echo";
	s[1] = "/bin/pwd";
	s[2] = "/bin/env";
	if (!is_builtin(token->cmd))
		return (status);
	if (strcmp(token->cmd, "cd") == 0)
		exec_cd(token, mini);
	else if ((strcmp(token->cmd, "echo") == 0) || !strcmp(token->cmd, s[0]))
		exec_echo(token, mini);
	else if ((strcmp(token->cmd, "pwd") == 0) || !strcmp(token->cmd, s[1]))
		exec_pwd(token);
	else if (strcmp(token->cmd, "export") == 0)
		exec_export(token, mini);
	else if (strcmp(token->cmd, "unset") == 0)
		exec_unset(token, mini);
	else if ((strcmp(token->cmd, "env") == 0) || !strcmp(token->cmd, s[2]))
		exec_env(token, mini);
	else if (strcmp(token->cmd, "exit") == 0)
		exec_exit(token);
	return (status);
}
