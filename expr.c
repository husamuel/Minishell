#include "minishell.h"

static int evaluate_term(const char *term, t_mini *ms) {
    if (ft_strcmp((char *)term, "$?") == 0) {
        return ms->exit_status;
    }
    
    int value = ft_atoi(term);
    int i = 0;
    while (term[i]) {
        if (!ft_isdigit(term[i])) {
            ft_putstr_fd("expr: invalid value: '", 2);
            ft_putstr_fd((char *)term, 2);
            ft_putstr_fd("'\n", 2);
            return 0;
        }
        i++;
    }
    
    return value;
}

static int process_expression(t_token *tokens, t_mini *ms) {
    int result = 0;
    t_token *current = tokens;
    
    if (current) {
        result = evaluate_term(current->cmd, ms);
        current = current->next;
    }
    
    while (current) {
        if (ft_strcmp(current->cmd, "+") == 0) {
            current = current->next;
            if (current) {
                result += evaluate_term(current->cmd, ms);
                current = current->next;
            } else {
                ft_putstr_fd("expr: missing operand after '+'\n", 2);
                break;
            }
        } else if (ft_strcmp(current->cmd, "-") == 0) {
            current = current->next;
            if (current) {
                result -= evaluate_term(current->cmd, ms);
                current = current->next;
            } else {
                ft_putstr_fd("expr: missing operand after '-'\n", 2);
                break;
            }
        } else if (ft_strcmp(current->cmd, "*") == 0) {
            current = current->next;
            if (current) {
                result *= evaluate_term(current->cmd, ms);
                current = current->next;
            } else {
                ft_putstr_fd("expr: missing operand after '*'\n", 2);
                break;
            }
        } else if (ft_strcmp(current->cmd, "/") == 0) {
            current = current->next;
            if (current) {
                int divisor = evaluate_term(current->cmd, ms);
                if (divisor == 0) {
                    ft_putstr_fd("expr: division by zero\n", 2);
                    return 0;
                }
                result /= divisor;
                current = current->next;
            } else {
                ft_putstr_fd("expr: missing operand after '/'\n", 2);
                break;
            }
        } else if (ft_strcmp(current->cmd, "%") == 0) {
            current = current->next;
            if (current) {
                int divisor = evaluate_term(current->cmd, ms);
                if (divisor == 0) {
                    ft_putstr_fd("expr: modulo by zero\n", 2);
                    return 0;
                }
                result %= divisor;
                current = current->next;
            } else {
                ft_putstr_fd("expr: missing operand after '%'\n", 2);
                break;
            }
        } else {
            ft_putstr_fd("expr: unknown operator '", 2);
            ft_putstr_fd(current->cmd, 2);
            ft_putstr_fd("'\n", 2);
            break;
        }
    }
    
    return result;
}

static void mark_tokens_as_processed(t_token *start) {
    t_token *current = start;
    while (current) {
        current->type = CMD_NONE;
        current = current->next;
    }
}

void process_expr_command(t_token *current, t_mini *ms) {
    if (!current->next) {
        ft_putstr_fd("expr: missing arguments\n", 2);
        ms->exit_status = 1;
        return;
    }
    
    t_mini temp_ms = *ms;
    
    int result = process_expression(current->next, &temp_ms);
    
    ft_putnbr_fd(result, 1);
    ft_putstr_fd("\n", 1);
    
    ms->exit_status = result;
    
    mark_tokens_as_processed(current->next);
}

void expr_verify(t_token *token) {
    if (!token || !token->cmd) {
        ft_putstr_fd("expr: invalid expression\n", 2);
    }
}
