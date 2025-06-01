/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handler_validation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:11:20 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/01 10:11:12 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	ft_report_unclosed_err(int in_single, int in_double)
{
	if (in_single)
	{
		ft_putstr_fd("minishell: syntax error: unclosed single quotes\n", 2);
		return (1);
	}
	else if (in_double)
	{
		ft_putstr_fd("minishell: syntax error: unclosed double quotes\n", 2);
		return (1);
	}
	return (0);
}

int	check_unclosed_quotes(const char *line)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (line[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		i++;
	}
	return (ft_report_unclosed_err(in_single_quote, in_double_quote));
}

int	handle_input_line(char *line)
{
	if (check_unclosed_quotes(line))
	{
		g_exit_status = 2;
		return (0);
	}
	return (1);
}
