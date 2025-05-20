/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:11:07 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/20 18:10:13 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	count_till_char(char *input, int *i, char c);
char	*get_end_arg(char *input, int *i);
char	*append_info_to_var(char *var, char *input, int *i, t_mini *mini);
char	*get_2var(char *input, int i);

char	*get_end_arg(char *input, int *i)
{
	char	*arg;
	int		start;

	if (input[*i] != '=')
		return (NULL);
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != ' ')
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			count_till_char(input, i, input[*i]);
		(*i)++;
	}
	arg = ft_substr(input, start, *i - start);
	return (arg);
}

char	*append_info_to_var(char *var, char *input, int *i, t_mini *mini)
{
	t_env	*exp;
	char	*join;
	char	*arg;

	exp = find_node(var, mini->export);
	arg = get_end_arg(input, i);
	if (exp && exp->content)
	{
		join = ft_strjoin(exp->content, arg + ft_strlen(var) + 2);
		free(arg);
		return (join);
	}
	join = ft_strjoin(var, arg + ft_strlen(var) + 1);
	free(arg);
	return (join);
}

char	*get_2var(char *input, int i)
{
	char	*content;
	char	*arg;
	int		j;

	j = 0;
	content = ft_strdup(input);
	content[i] = '\0';
	arg = ft_strdup(content);
	i = 0;
	while (content[j])
	{
		if (content[j] != '\'' && content[j] != '\"')
		{
			arg[i] = content[j];
			i++;
		}
		j++;
	}
	arg[i] = '\0';
	free(content);
	return (arg);
}

void	count_till_char(char *input, int *i, char c)
{
	if (c == '\'' || c == '\"')
		(*i)++;
	while (input[*i] && input[*i] != c)
		(*i)++;
}
