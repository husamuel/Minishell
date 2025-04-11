#include "./../../minishell.h"

void	count_till_char(char *input, int *i, char c);
char	*get_end_arg(char *input, int *i);
char	*append_info_to_var(char *var, char *input, int *i, t_mini *mini);
char	*get_2var(char *input, int i);

// finds end of argument / creates string without quotes
char	*get_end_arg(char *input, int *i)
{
	char	*arg;

	while (input[*i] && input[*i] != ' ')
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			count_till_char(input, i, input[*i]);
		(*i)++;
	}
	arg = get_2var(input, *i);
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

// create string with i elements via malloc / taking out the quotes
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

// increment i till found char return position char
void	count_till_char(char *input, int *i, char c)
{
	if (c == '\'' || c == '\"')
		(*i)++;
	while (input[*i] && input[*i] != c)
		(*i)++;
}
