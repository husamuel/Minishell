/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:33:40 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/04 13:09:42 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	execute_external_builtin(t_token *token, t_mini *ms)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (setup_redirections(token) == -1)
			exit(1);
		exec_builtin(token, ms);
		exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (is_core_dumped(status));
	}
	else
	{
		perror("fork");
		return (1);
	}
	return (0);
}

static int	compare_builtin(const char *cmd, const char *builtin)
{
	return (strcmp(cmd, builtin) == 0);
}

int	is_builtin(const char *cmd)
{
	static const char	*builtins[] = {
		"cd", "echo", "pwd", "export", "unset", "env", "exit",
		"/bin/pwd", "/bin/env", "/bin/echo", NULL
	};
	int					i;

	if (!cmd)
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (compare_builtin(cmd, builtins[i]))
			return (1);
		i++;
	}
	return (0);
}

int	exec_basic_builtins(t_token *token, t_mini *mini)
{
	if (compare_builtin(token->cmd, "cd"))
		exec_cd(token, mini);
	else if (compare_builtin(token->cmd, "export"))
		exec_export(token, mini);
	else if (compare_builtin(token->cmd, "unset"))
		exec_unset(token, mini);
	else if (compare_builtin(token->cmd, "exit"))
		exec_exit(token);
	else
		return (127);
	return (0);
}

int	exec_system_builtins(t_token *token, t_mini *mini, char **paths)
{
	if (compare_builtin(token->cmd, "echo")
		|| compare_builtin(token->cmd, paths[0]))
		exec_echo(token, mini);
	else if (compare_builtin(token->cmd, "pwd")
		|| compare_builtin(token->cmd, paths[1]))
		exec_pwd(token);
	else if (compare_builtin(token->cmd, "env")
		|| compare_builtin(token->cmd, paths[2]))
		exec_env(token, mini);
	else
		return (127);
	return (0);
}
