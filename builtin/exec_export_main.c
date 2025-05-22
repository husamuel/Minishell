/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:25:38 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 12:43:22 by husamuel         ###   ########.fr       */
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

void	exec_export(t_token *token, t_mini *mini)
{
	char	*input;
	int		i;

	i = 0;
	order_var(mini);
	if (!token->next)
		print_export(mini->export);
	input = mini->input + 6;
	if (!check_quotes(input))
	{
		printf("Not closed quotes\n");
		return ;
	}
	while (*input)
	{
		while (*input == ' ')
			input++;
		i = parsing_export(input, mini);
		if (i > (int)ft_strlen(input))
			i = ft_strlen(input);
		input += i;
	}
}

static char	*ft_extract_var_name(char *input, int *i)
{
	char	*end;

	end = "= ";
	while (input[*i] && !ft_strchr(end, input[*i])
		&& ft_strncmp(input + *i, "+=", 2) != 0)
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			count_till_char(input, i, input[*i]);
		(*i)++;
	}
	return (get_2var(input, *i));
}

int	parsing_export(char *input, t_mini *mini)
{
	int		i;
	char	*var;
	char	*arg;

	arg = NULL;
	i = 0;
	var = ft_extract_var_name(input, &i);
	if (ft_strncmp(input + i, "+=", 2) == 0)
		arg = append_info_to_var(var, input, &i, mini);
	else if (input[i] == '=')
		arg = get_end_arg(input, &i);
	if (arg)
		arg = export_expand(arg, mini);
	var = export_expand(var, mini);
	if (syntax_export(var, arg))
		create_export(var, arg, mini);
	free(var);
	free(arg);
	return (i + 1);
}

void	create_export(char *var, char *content, t_mini *mini)
{
	t_env	*export;

	export = find_node(var, mini->export);
	if (export)
	{
		if (content)
		{
			free(export->content);
			export->content = ft_strdup(content);
		}
		return ;
	}
	append_node(ft_strdup(var), ft_strdup(content), mini->export);
}
