#include "./../../includes/minishell.h"

t_token_type get_token_type(char *str)
{
    if (strcmp(str, "|") == 0)
        return PIPE;
    else if (strcmp(str, ">") == 0 || strcmp(str, "<") == 0)
        return REDIRECTION;
    else if (str[0] == '-')
        return ARGUMENT;
    else if (str[0] != '\0')
        return COMMAND;
    else
        return UNKNOWN;
}

const char *token_type_to_string(t_token_type type)
{
    static const char *token_type_strings[] = {
        "COMMAND",
        "ARGUMENT",
        "PIPE",
        "REDIRECTION",
        "UNKNOWN"
    };

    if (type < 0 || type >= UNKNOWN) // Garantir que o índice é válido
        return "UNKNOWN";
    return token_type_strings[type];
}
