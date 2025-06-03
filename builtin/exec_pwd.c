/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:09:06 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/03 19:47:01 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	exec_pwd(t_token *token)
{
	char	*current_path;
	char	buffer[BUFFER_SIZE];

	(void)token;
	current_path = getcwd(buffer, BUFFER_SIZE);
	if (!current_path)
	{
		perror("pwd");
		return ;
	}
	printf("%s\n", current_path);
	setup_signals();
}

void	create_export(char *var, char *content, t_mini *mini)
{
	t_env	*export;
	char	*value;

	export = find_node(var, mini->export);
	if (content)
		value = ft_strdup(content);
	else
		value = NULL;
	if (export)
	{
		free(export->content);
		export->content = value;
		return ;
	}
	append_node(ft_strdup(var), value, mini->export);
}
