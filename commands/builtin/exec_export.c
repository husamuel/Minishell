#include "./../../minishell.h"

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

int	parsing_export(char *input, t_mini *mini)
{
	int		i;
	char	*end;
	char	*var;
	char	*arg;

	arg = NULL;
	i = 0;
	end = "= ";
	while (input[i] && !ft_strchr(end, input[i])
		&& ft_strncmp(input + i, "+=", 2) != 0)
	{
		if (input[i] == '\'' || input[i] == '\"')
			count_till_char(input, &i, input[i]);
		i++;
	}
	var = get_2var(input, i);
	if (ft_strncmp(input + i, "+=", 2) == 0)
		arg = append_info_to_var(var, input, &i, mini);
	else if (input[i] == '=')
		arg = get_end_arg(input, &i);
	if (arg)
		arg = export_expand(arg, mini);
	var = export_expand(var, mini);
	if (syntax_export(var, arg))
		create_export(var, arg, mini);
	return (i + 1);
}

char	*export_expand(char *input, t_mini *mini)
{
	char	*temp;
	
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

char	*get_next_arg(char *input, t_mini *mini)
{
	int		i;
	char	*temp;
	char	*beg;
	char	*content;
	char	*res;
	char	*final;

	i = 0;
	temp = ft_strchr(input, '$');
	beg = get_first_arg(input);
	content = get_var_name(temp + 1, &i, mini);
	if (content)
	{
		res = ft_strjoin(beg, content);
		free_2strings(beg, content);
		final = ft_strjoin(res, temp + i + 1);
		free(res);
	}
	else
	{
		final = ft_strjoin(beg, temp + i + 1);
		free(beg);
	}
	return (final);
}

// return var content with malloc
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

void	create_export(char *var, char *content, t_mini *mini)
{
	t_env	*export;

	export = find_node(var, mini->export);
	if (export)
	{
		if (content)
		{
			free(export->content);
			export->content = content;
		}
		return ;
	}
	append_node(var, content, mini->export);
}
