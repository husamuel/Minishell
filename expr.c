#include "minishell.h"

static int	evaluate_term(const char *term, t_mini *ms)
{
	int	value;
	int	i;

	if (ft_strcmp((char *)term, "$?") == 0)
		return (ms->exit_status);
	value = ft_atoi(term);
	i = 0;
	while (term[i])
	{
		if (!ft_isdigit(term[i]))
		{
			ft_putstr_fd("expr: invalid value: '", 2);
			ft_putstr_fd((char *)term, 2);
			ft_putstr_fd("'\n", 2);
			return (0);
		}
		i++;
	}
	return (value);
}

static int	handle_operator(char *op, t_token **current, int result, t_mini *ms)
{
	int	divisor;

	(*current) = (*current)->next;
	if (!(*current))
	{
		ft_putstr_fd("expr: missing operand after '", 2);
		ft_putstr_fd(op, 2);
		ft_putstr_fd("'\n", 2);
		return (result);
	}
	if (ft_strcmp(op, "+") == 0)
		return (result + evaluate_term((*current)->cmd, ms));
	else if (ft_strcmp(op, "-") == 0)
		return (result - evaluate_term((*current)->cmd, ms));
	else if (ft_strcmp(op, "*") == 0)
		return (result * evaluate_term((*current)->cmd, ms));
	else if (ft_strcmp(op, "/") == 0 || ft_strcmp(op, "%") == 0)
	{
		divisor = evaluate_term((*current)->cmd, ms);
		if (divisor == 0)
		{
			ft_putstr_fd("expr: ", 2);
			ft_putstr_fd(op[0] == '/' ? "division" : "modulo", 2);
			ft_putstr_fd(" by zero\n", 2);
			return (0);
		}
		return (op[0] == '/' ? result / divisor : result % divisor);
	}
	return (result);
}

static int	process_expression(t_token *tokens, t_mini *ms)
{
	int		result;
	t_token	*current;
	char	*operator;

	result = 0;
	current = tokens;
	if (current)
	{
		result = evaluate_term(current->cmd, ms);
		current = current->next;
	}
	while (current)
	{
		operator = current->cmd;
		if (ft_strcmp(operator, "+") == 0 || ft_strcmp(operator, "-") == 0
			|| ft_strcmp(operator, "*") == 0 || ft_strcmp(operator, "/") == 0
			|| ft_strcmp(operator, "%") == 0)
			result = handle_operator(operator, &current, result, ms);
		else
		{
			ft_putstr_fd("expr: unknown operator '", 2);
			ft_putstr_fd(current->cmd, 2);
			ft_putstr_fd("'\n", 2);
			break ;
		}
		current = current->next;
	}
	return (result);
}

static void	mark_tokens_as_processed(t_token *start)
{
	t_token	*current;

	current = start;
	while (current)
	{
		current->type = CMD_NONE;
		current = current->next;
	}
}

void	process_expr_command(t_token *current, t_mini *ms)
{
	t_mini	temp_ms;
	int		result;

	temp_ms = *ms;
	if (!current->next)
	{
		ft_putstr_fd("expr: missing arguments\n", 2);
		ms->exit_status = 1;
		return ;
	}
	result = process_expression(current->next, &temp_ms);
	ft_putnbr_fd(result, 1);
	ft_putstr_fd("\n", 1);
	ms->exit_status = result;
	mark_tokens_as_processed(current->next);
}

void	expr_verify(t_token *token)
{
	if (!token || !token->cmd)
		ft_putstr_fd("expr: invalid expression\n", 2);
}