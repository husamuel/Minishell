#include "./../../minishell.h"
char *print_echo(char *input, int *i, char *var, t_mini *mini);
void echo_others(t_token *next, int i, t_mini *mini, char *input);
void echo_dollar(int *i, char *input, t_mini *mini);
int check_nl_echo(char *s);

int has_redirection(t_token *token)
{
    t_token *current = token;
    while (current)
    {
        if (current->type == CMD_REDIRECT_OUT || current->type == CMD_REDIRECT_IN || 
            current->type == CMD_HEREDOC)
            return 1;
        current = current->next;
    }
    return 0;
}

char *ft_custom_strstr(const char *haystack, const char *needle)
{
    size_t needle_len;
    
    if (!haystack || !needle)
        return NULL;
        
    needle_len = ft_strlen(needle);
    
    if (needle_len == 0)
        return (char *)haystack;
        
    while (*haystack)
    {
        if (ft_strncmp(haystack, needle, needle_len) == 0)
            return (char *)haystack;
        haystack++;
    }
    
    return NULL;
}

char *ft_custom_substr(char const *s, unsigned int start, size_t len)
{
    char *result;
    size_t i;
    
    if (!s)
        return NULL;
        
    if (start >= ft_strlen(s))
        return ft_strdup("");
        
    if (len > ft_strlen(s) - start)
        len = ft_strlen(s) - start;
        
    result = malloc(sizeof(char) * (len + 1));
    if (!result)
        return NULL;
        
    i = 0;
    while (i < len && s[start + i])
    {
        result[i] = s[start + i];
        i++;
    }
    result[i] = '\0';
    
    return result;
}

void exec_echo(t_token *token, t_mini *mini)
{
    t_token *next;
    char *content;
    int has_redirect;
    
    has_redirect = has_redirection(token);
    
    next = token->next;
    if (!next)
        printf("\n");
    else if (ft_strcmp(next->cmd, "~") == 0)
    {
        content = getenv("HOME");
        printf("%s\n", content);
    }
    else
    {
        char *input_start = mini->input + 5;
        
        char *redirect_pos = NULL;
        if (has_redirect)
        {
            redirect_pos = ft_custom_strstr(input_start, " >");
            if (!redirect_pos)
                redirect_pos = ft_custom_strstr(input_start, " >>");
            if (!redirect_pos)
                redirect_pos = ft_custom_strstr(input_start, " <");
            if (!redirect_pos)
                redirect_pos = ft_custom_strstr(input_start, " <<");
        }
        
        if (has_redirect && redirect_pos)
        {
            int len_to_redirect = redirect_pos - input_start;
            char *temp_input = ft_custom_substr(input_start, 0, len_to_redirect);
            
            printf("\n");
            free(temp_input);
        }
        else
        {
            echo_others(next, 0, mini, input_start);
        }
    }
}

int check_nl_echo(char *s)
{
    int i = 0;

    if (!s || s[i] != '-' || s[i + 1] != 'n')
        return 0;

    i++;
    while (s[i] == 'n')
        i++;

    if (s[i] == '\0')
        return 1;

    return 0;
}

void echo_others(t_token *next, int i, t_mini *mini, char *input)
{
    int nl_flag = 0;

    while (next && ft_strncmp(next->cmd, "-n", 2) == 0 && check_nl_echo(next->cmd))
    {
        next = next->next;
        nl_flag = 1;
    }

    if (next)
    {
        while (*input == ' ')
            input++;
        char *next_pos = ft_custom_strstr(input, next->cmd);
        if (next_pos)
            input = next_pos;
    }

    while (input && input[i])
    {
        if (input[i] == '\"' || ((input[i - 1] != '\"' && input[i + 1] != '\"') && input[i] == '\''))
        {
            i++;
        }
        else if (input[i - 1] != '\'' && input[i] == '$')
        {
            echo_dollar(&i, input, mini);
        }
        else
        {
            printf("%c", input[i]);
            i++;
        }
    }

    if (!nl_flag)
        printf("\n");
}


void echo_dollar(int *i, char *input, t_mini *mini)
{
    char *var;
    var = NULL;
    if (input[*i + 1] == '?')
    {
        printf("0");
        *i += 2;
        if (input[*i] == ' ')
            (*i)++;
        return;
    }
    else if (ft_isdigit(input[*i + 1]))
    {
        if (input[*i + 1] == '0')
            printf("minishell");
        *i += 2;
        if (input[*i] == ' ' && input[*i - 1] != '0')
            (*i)++;
    }
    else if (ft_isalpha(input[*i + 1]))
    {
        var = ft_strdup(&input[++(*i)]);
        if (!print_echo(input, i, var, mini) && input[*i] == ' ')
            (*i)++;
    }
}

char *print_echo(char *input, int *i, char *var, t_mini *mini)
{
    char *s;
    int j;
    j = 0;
    while (ft_isalnum(var[j]))
        j++;
    var[j] = '\0';
    s = expand_var(var, mini->export);
    free(var);
    if (s)
    {
        printf("%s", s);
        free(s);
    }
    while (ft_isalnum(input[*i]))
        (*i)++;
    return (s);
}