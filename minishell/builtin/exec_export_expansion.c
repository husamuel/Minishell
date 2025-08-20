/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_expansion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:27:07 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/03 19:34:38 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

// file export_3.c
int		syntax_export(char *var, char *arg);
int		check_quotes(char *input);
int		count_char(char *input, char c);

// file export_4.c
char	*append_info_to_var(char *var, char *input, int *i, t_mini *mini);
char	*get_2var(char *input, int i);
char	*get_end_arg(char *input, int *i);
void	count_till_char(char *input, int *i, char c);

void	create_export(char *var, char *content, t_mini *mini);
int		parsing_export(char *input, t_mini *mini);

char	*get_first_arg(char *input);
char	*get_next_arg(char *input, t_mini *mini);
char	*export_expand(char *input, t_mini *mini);
char	*get_var_name(char *input, int *a, t_mini *mini);

char	*export_expand(char *input, t_mini *mini)
{
	char	*temp;

	if (!ft_strchr(input, '$'))
		return (ft_strdup(input));
	while (input)
	{
		temp = ft_strchr(input, '$');
		if (!temp)
			break ;
		temp = input;
		input = get_next_arg(input, mini);
		free(temp);
	}
	return (input);
}

void	ft_init_arg_parts(t_arg *parts, char *input, t_mini *mini)
{
	parts->i = 0;
	parts->temp = ft_strchr(input, '$');
	parts->beg = get_first_arg(input);
	if (parts->temp)
		parts->content = get_var_name(parts->temp + 1, &parts->i, mini);
	else
		parts->content = NULL;
}

char	*get_next_arg(char *input, t_mini *mini)
{
	t_arg	parts;
	char	*res;
	char	*final;

	ft_init_arg_parts(&parts, input, mini);
	if (parts.content)
	{
		res = ft_strjoin(parts.beg, parts.content);
		free_2strings(parts.beg, parts.content);
		final = ft_strjoin(res, parts.temp + parts.i + 1);
		free(res);
	}
	else
	{
		final = ft_strjoin(parts.beg, parts.temp + parts.i + 1);
		free(parts.beg);
	}
	return (final);
}

char	*get_var_name(char *input, int *a, t_mini *mini)
{
	char	*var;
	char	*content;
	int		i;

	i = 0;
	if (ft_isdigit(input[0]))
	{
		(*a)++;
		if (input[0] == '0')
			return (ft_strdup("minishell"));
		return (NULL);
	}
	var = ft_strdup(input);
	while (ft_isalnum(var[i]))
		i++;
	var[i] = 0;
	content = expand_var(var, mini->export);
	free(var);
	*a = i;
	return (content);
}

char	*get_first_arg(char *input)
{
	char	*beg;
	char	*temp;

	beg = ft_strdup(input);
	temp = ft_strchr(beg, '$');
	temp[0] = '\0';
	return (beg);
}
