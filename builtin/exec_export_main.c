/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:25:38 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/08 12:24:03 by husamuel         ###   ########.fr       */
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

static int	is_valid_identifier(char *input, int *j)
{
	int	start;

	start = *j;
	if (!ft_isalpha(input[*j]) && input[*j] != '_')
	{
		printf("minishell: export: `%.*s': not a valid identifier\n",
			(int)(ft_strchrnul(input + start, ' ') - (input + start)),
			input + start);
		return (0);
	}
	while (input[*j] && input[*j] != '='
		&& !(input[*j] == '+' && input[*j + 1] == '=')
		&& !ft_isspace(input[*j]))
	{
		if (!ft_isalnum(input[*j]))
		{
			printf("minishell: export: `%.*s': not a valid identifier\n",
				(int)(ft_strchrnul(input + start, ' ') - (input + start)),
				input + start);
			return (0);
		}
		(*j)++;
	}
	return (1);
}

static int	process_export_args(char *input, t_mini *mini)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (input[j])
	{
		while (ft_isspace(input[j]))
			j++;
		start = j;
		if (!is_valid_identifier(input, &j))
			return (0);
		i = parsing_export(input + start, mini);
		if (i > (int)ft_strlen(input + start))
			i = ft_strlen(input + start);
		j = start + i;
		while (ft_isspace(input[j]))
			j++;
	}
	return (1);
}

void	exec_export(t_token *token, t_mini *mini)
{
	char	*input;

	order_var(mini);
	if (!token->next)
		return (print_export(mini->export));
	input = mini->input + 6;
	if (!check_quotes(input))
		return ((void)printf("Not closed quotes\n"));
	if (!process_export_args(input, mini))
		return ;
	setup_signals();
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

	i = 0;
	arg = NULL;
	var = ft_extract_var_name(input, &i);
	if (ft_strncmp(input + i, "+=", 2) == 0)
		arg = append_info_to_var(var, input, &i, mini);
	else if (input[i] == '=')
		arg = get_end_arg(input, &i);
	if (arg)
		arg = expand_and_free(arg, mini);
	var = expand_and_free(var, mini);
	if (syntax_export(var, arg))
		create_export(var, arg, mini);
	free(var);
	free(arg);
	return (i + 1);
}
