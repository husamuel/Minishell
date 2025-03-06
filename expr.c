#include "minishell.h"

static int evaluate_term(const char *term, t_mini *ms) {
    if (strcmp(term, "$?") == 0) {
        return ms->exit_status;
    }
    
    char *endptr;
    int value = (int)strtol(term, &endptr, 10);
    
    // Se não for um número válido
    if (*endptr != '\0') {
        fprintf(stderr, "expr: valor inválido: '%s'\n", term);
        return 0;
    }
    
    return value;
}

// Função para processar uma expressão
static int process_expression(t_token *tokens, t_mini *ms) {
    int result = 0;
    t_token *current = tokens;
    
    // Primeiro token deve ser um termo
    if (current) {
        result = evaluate_term(current->cmd, ms);
        current = current->next;
    }
    
    // Processa operações
    while (current) {
        // Verifica operador
        if (strcmp(current->cmd, "+") == 0) {
            current = current->next;
            if (current) {
                result += evaluate_term(current->cmd, ms);
                current = current->next;
            } else {
                fprintf(stderr, "expr: falta operando após '+'\n");
                break;
            }
        } else if (strcmp(current->cmd, "-") == 0) {
            current = current->next;
            if (current) {
                result -= evaluate_term(current->cmd, ms);
                current = current->next;
            } else {
                fprintf(stderr, "expr: falta operando após '-'\n");
                break;
            }
        } else if (strcmp(current->cmd, "*") == 0) {
            current = current->next;
            if (current) {
                result *= evaluate_term(current->cmd, ms);
                current = current->next;
            } else {
                fprintf(stderr, "expr: falta operando após '*'\n");
                break;
            }
        } else if (strcmp(current->cmd, "/") == 0) {
            current = current->next;
            if (current) {
                int divisor = evaluate_term(current->cmd, ms);
                if (divisor == 0) {
                    fprintf(stderr, "expr: divisão por zero\n");
                    return 0;
                }
                result /= divisor;
                current = current->next;
            } else {
                fprintf(stderr, "expr: falta operando após '/'\n");
                break;
            }
        } else if (strcmp(current->cmd, "%") == 0) {
            current = current->next;
            if (current) {
                int divisor = evaluate_term(current->cmd, ms);
                if (divisor == 0) {
                    fprintf(stderr, "expr: módulo por zero\n");
                    return 0;
                }
                result %= divisor;
                current = current->next;
            } else {
                fprintf(stderr, "expr: falta operando após '%%'\n");
                break;
            }
        } else {
            fprintf(stderr, "expr: operador desconhecido '%s'\n", current->cmd);
            break;
        }
    }
    
    return result;
}

// Função para marcar tokens como processados
static void mark_tokens_as_processed(t_token *start) {
    t_token *current = start;
    while (current) {
        current->type = CMD_NONE;  // Marca como processado
        current = current->next;
    }
}

// Função principal para processar o comando expr
void process_expr_command(t_token *current, t_mini *ms) {
    // Verifica se há argumentos
    if (!current->next) {
        fprintf(stderr, "expr: argumentos ausentes\n");
        ms->exit_status = 1;
        return;
    }
    
    // Processa a expressão
    int result = process_expression(current->next, ms);
    
    // Imprime o resultado
    printf("%d\n", result);
    
    // Define o status de saída
    ms->exit_status = result;
    
    // Marca tokens subsequentes como processados
    mark_tokens_as_processed(current->next);
}

// Função para verificar a validade da expressão
void expr_verify(t_token *token) {
    // Esta função pode ser expandida para verificações adicionais
    if (!token || !token->cmd) {
        fprintf(stderr, "expr: expressão inválida\n");
    }
}