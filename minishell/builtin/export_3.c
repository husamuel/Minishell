/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:10:55 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/22 12:43:28 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int		syntax_export(char *var, char *arg);
int		check_quotes(char *input);
int		count_char(char *input, char c);
void	print_error(char *var, char *arg);
void	free_2strings(char *var, char *arg);

// count number of char on a string
int	count_char(char *input, char c)
{
	int	i;

	i = 0;
	while (*input)
	{
		if (*input == c)
			i++;
		input++;
	}
	return (i);
}

// check for unclosed quotes not even number
int	check_quotes(char *input)
{
	int	i;
	int	j;

	i = count_char(input, '\'');
	if (i % 2 != 0)
		return (0);
	j = count_char(input, '\"');
	if (j % 2 != 0)
		return (0);
	return (1);
}

// checks sintax of var on export prints error frees strings
int	syntax_export(char *var, char *arg)
{
	int	i;

	i = 0;
	if (ft_isdigit(var[i]))
	{
		print_error(var, arg);
		free_2strings(var, arg);
		return (0);
	}
	while (var[i])
	{
		if (!ft_isalnum(var[i]))
		{
			print_error(var, arg);
			free_2strings(var, arg);
			return (0);
		}
		i++;
	}
	return (1);
}

// print custom error for export
void	print_error(char *var, char *arg)
{
	if (arg)
		printf("minishell: export: `%s': not a valid identifier\n", arg);
	else
		printf("minishell: export: `%s': not a valid identifier\n", var);
}

// free 2 strings
void	free_2strings(char *var, char *arg)
{
	if (var)
		free(var);
	if (arg)
		free(arg);
}
